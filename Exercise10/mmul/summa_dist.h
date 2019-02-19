#ifndef SUMMA_DIST_H
#define SUMMA_DIST_H

#include <mpi.h>

#include "matrix.h"
#include "gsl/gsl"
#include "naive_seq.h"
#include "summa_seq.h"
#include <thread>
#include <iterator>

enum tags
{
    block_a,
    block_b,
    column_a,
    row_b,
    block_c
};

struct summa_distributed { size_t blocks = 4; bool print = false; };

inline size_t get_processor_rank(const size_t y, const size_t x, const size_t procs_per_dim)
{
    return y * procs_per_dim + x;
}

template<typename T>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, const summa_distributed opt)
{
    Expects(a.n() == b.n());

    const auto n = a.n();

    auto rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto num_procs = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    const auto blocks_per_dim = get_num_blocks_per_dim(n, num_procs);

    const auto num_blocks = blocks_per_dim * blocks_per_dim;

    if (rank >= gsl::narrow<int>(num_blocks)) return b;

    if (blocks_per_dim == 1)
    {
        return multiply(a, b, naive_sequential());
    }

    const auto block_size = n / blocks_per_dim;

    block_t<T> block =
    {
        matrix<T>(block_size),
        matrix<T>(block_size),
        matrix<T>(block_size, 0)
    };

    if(rank == 0)
    {
        block_grid_t<T> blocks(blocks_per_dim);

        for (auto& row : blocks)
        {
            row.resize(blocks_per_dim,
            {
                matrix<T>(block_size),
                matrix<T>(block_size),
                matrix<T>(block_size, 0)
            });
        }

        // build blocks  

        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                const auto block_y = i / block_size;
                const auto block_x = j / block_size;

                auto& current_block = blocks[block_y][block_x];

                const auto block_offset_x = block_x * block_size;
                const auto local_x = j - block_offset_x;

                const auto block_offset_y = block_y * block_size;
                const auto local_y = i - block_offset_y;

                current_block[0](local_y, local_x) = a(i, j);
                current_block[1](local_y, local_x) = b(i, j);
            }
        }

        if (opt.print && n <= 8)
        {
            std::cout << std::endl;
            std::cout << std::endl;

            print_blocks(blocks);
        }

   
        // assign initial data distribution 

        std::vector<MPI_Request> requests;

        for (auto y = 0l; y < gsl::narrow<long>(blocks_per_dim); ++y)
        {
            for (auto x = 0l; x < gsl::narrow<long>(blocks_per_dim); ++x)
            {
                if (y == 0 && x == 0) continue;

                const auto src_a_y = y % blocks_per_dim;
                const auto src_a_x = (-x - y) % blocks_per_dim;

                const auto src_b_y = (-x - y) % blocks_per_dim;
                const auto src_b_x = x % blocks_per_dim;

                const auto& local_a = blocks[src_a_y][src_a_x][0];
                const auto& local_b = blocks[src_b_y][src_b_x][1];

                const auto dest_rank = get_processor_rank(y, x, blocks_per_dim);

                requests.push_back(MPI_Request());

                MPI_Isend(local_a.begin(),
                    gsl::narrow<int>(local_a.size()),
                    MPI_DOUBLE, 
                    gsl::narrow<int>(dest_rank),
                    block_a, 
                    MPI_COMM_WORLD,
                    &requests.back());

                requests.push_back(MPI_Request());

                MPI_Isend(local_b.begin(),
                    gsl::narrow<int>(local_b.size()),
                    MPI_DOUBLE, 
                    gsl::narrow<int>(dest_rank),
                    block_b, 
                    MPI_COMM_WORLD,
                    &requests.back());
            }
        }

        block = blocks[0][0];

        MPI_Waitall(gsl::narrow<int>(requests.size()), requests.data(), nullptr);
    }
    else
    {
        MPI_Request requests[2];

        // receive block of matrix a
        MPI_Irecv(block[0].begin(),
            gsl::narrow<int>(block[0].size()),
            MPI_DOUBLE, 0, block_a, MPI_COMM_WORLD,
            &requests[0]);

        // receive block of matrix b
        MPI_Irecv(block[1].begin(),
            gsl::narrow<int>(block[1].size()),
            MPI_DOUBLE, 0, block_b, MPI_COMM_WORLD, 
            &requests[1]);

        MPI_Waitall(2, requests, nullptr);
    }
      
    constexpr auto mod = [](const auto a, const auto b)
    {
        return (b + (a % b)) % b;
    };

    const auto proc_y = gsl::narrow<long>(rank / blocks_per_dim);
    const auto proc_x = gsl::narrow<long>(rank % blocks_per_dim);

    const auto neighbour_bottom_rank =
        get_processor_rank(mod(proc_y + 1, blocks_per_dim), proc_x, blocks_per_dim);

    const auto neighbour_right_rank =
        get_processor_rank(proc_y, mod(proc_x + 1, blocks_per_dim), blocks_per_dim);

    const auto neighbour_left_rank =
        get_processor_rank(proc_y, mod(proc_x - 1, blocks_per_dim), blocks_per_dim);

    const auto neighbour_top_rank =
        get_processor_rank(mod(proc_y - 1, blocks_per_dim), proc_x, blocks_per_dim);

    Ensures(neighbour_right_rank < num_blocks);
    Ensures(neighbour_bottom_rank < num_blocks);
    Ensures(neighbour_left_rank < num_blocks);
    Ensures(neighbour_top_rank < num_blocks);
    
    for (size_t it = 0; it < n; ++it)
    {
        //std::cout << "(" << rank << "): iteration " << it << std::endl;

        // current row/column
        const auto k = it % block_size;

        // send row/column ===================================================
        MPI_Request row_send_request;
        MPI_Isend(&block[1](k, 0),
            gsl::narrow<int>(block_size),
            MPI_DOUBLE,
            gsl::narrow<int>(neighbour_bottom_rank),
            row_b,
            MPI_COMM_WORLD,
            &row_send_request);

        std::vector<T> column(block_size);

        for (size_t i = 0; i < column.size(); ++i)
        {
            column[i] = block[0](i, k);
        }

        MPI_Request col_send_request;
        MPI_Isend(column.data(),
            gsl::narrow<int>(block_size),
            MPI_DOUBLE,
            gsl::narrow<int>(neighbour_right_rank),
            column_a,
            MPI_COMM_WORLD,
            &col_send_request);

      
        // ===================================================================

        //std::cout << "(" << rank << "): updating block" << std::endl;

        update_block(block, k);

        // recv row/column ===================================================
        
        MPI_Request row_recv_request;
        MPI_Irecv(&block[1](k, 0), 
            gsl::narrow<int>(block_size),
            MPI_DOUBLE,
            gsl::narrow<int>(neighbour_top_rank),
            row_b, 
            MPI_COMM_WORLD, 
            &row_recv_request);

        MPI_Request col_recv_request;
        MPI_Irecv(column.data(), 
            gsl::narrow<int>(block_size),
            MPI_DOUBLE,
            gsl::narrow<int>(neighbour_left_rank),
            column_a, 
            MPI_COMM_WORLD, 
            &col_recv_request);
 
        //std::cout << "(" << rank << "): receiving row" << std::endl;
        MPI_Wait(&row_recv_request, nullptr);

        //std::cout << "(" << rank << "): receiving col" << std::endl;
        MPI_Wait(&col_recv_request, nullptr);

        for (size_t i = 0; i < column.size(); ++i)
        {
            block[0](i, k) = column[i];
        }

        // ===================================================================
          
        // wait for send to complete
        //std::cout << "(" << rank << "): wait for row sending to complete" << std::endl;
        MPI_Wait(&row_send_request, nullptr);

        //std::cout << "(" << rank << "): wait for col sending to complete" << std::endl;
        MPI_Wait(&col_send_request, nullptr);
    }
 

    MPI_Barrier(MPI_COMM_WORLD);

    // send result ======================================================

    if(rank != 0)
    {
        MPI_Send(block[2].begin(),
            gsl::narrow<int>(block[2].size()),
            MPI_DOUBLE,
            0,
            block_c,
            MPI_COMM_WORLD);

        //std::this_thread::sleep_for(std::chrono::milliseconds(rank * 300));

        return b;
    }

    // gather result ====================================================

    //std::cout << "gathering final result" << std::endl;

    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    auto c = matrix<T>(n);

    for (auto y = 0; y < gsl::narrow<long>(blocks_per_dim); ++y)
    {
        for (auto x = 0; x < gsl::narrow<long>(blocks_per_dim); ++x)
        {     
            matrix<T> temp(block_size);

            if(x == 0 && y == 0)
            {
                temp = block[2];
            }
            else
            {
                MPI_Recv(temp.begin(),
                    gsl::narrow<int>(block_size * block_size),
                    MPI_DOUBLE,
                    gsl::narrow<int>(get_processor_rank(y, x, blocks_per_dim)),
                    block_c,
                    MPI_COMM_WORLD,
                    nullptr);
            }

            for (size_t i = 0; i < block_size; ++i)
            {
                std::copy(
                    &temp(i, 0), 
                    &temp(i + 1, 0), 
                    &c(y * block_size + i, x * block_size));
            }
        }
    }

    if (!opt.print || n > 8) return c;

    // print result =====================================================

    std::cout << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            std::cout << std::setfill(' ') << std::setw(2) << c(i, j) << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    return c;
}

#endif // SUMMA_DIST_H
