//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_MPI_STENCIL_H
#define PARALLELSYSTEMS_MPI_STENCIL_H

#include "grid.h"
#include <memory>

constexpr auto epsilon = 10;


class stencil
{
public:
    const int my_rank;
    int top_rank;
    int right_rank;
    int bottom_rank;
    int left_rank;


    const int num_blocks;
    const int elements_per_block;

    stencil(const int num_procs, const int my_rank, const grid& grid1) :
        my_rank(my_rank), 
        num_blocks(static_cast<int>(sqrt(num_procs))), 
        elements_per_block(n / num_blocks)
    {

        left_rank = grid1.idx_x() * num_blocks + grid1.idx_y() - 1;
        right_rank = grid1.idx_x() * num_blocks + grid1.idx_y() + 1;

        top_rank = (grid1.idx_x() - 1) * num_blocks + grid1.idx_y();
        bottom_rank = (grid1.idx_x() + 1) * num_blocks + grid1.idx_y();

        if (grid1.idx_x() == 0u)
        {
            top_rank = -1;
        }
        if (static_cast<int>(grid1.idx_x()) == num_blocks - 1)
        {
            bottom_rank = -1;
        }

        if(grid1.idx_y() == 0u)
        {
            left_rank = -1;
        }
        if(static_cast<int>(grid1.idx_y()) == num_blocks - 1)
        {
            right_rank = -1;
        }
    }


    //Does the job and returns the delta Epsilon accumulated over all comparisons.
    static double iteration(const grid& source, grid& target) 
    {
        auto epsilon = 0.0;

        for (auto i = source.left_x(); i < source.right_x(); i++) 
        {
            if (i == 0)     continue;
            if (i == n - 1) continue;

            for (auto j = source.top_y(); j < source.bottom_y(); j++) 
            {
                if (j == 0)     continue;
                if (j == n - 1) continue;
                  
                const auto current = source.get(i , j);
                const auto new_value = source.get_five(i , j);

                target.set(i , j , new_value);

                epsilon += std::abs(new_value - current);
            }
        }

        return epsilon;
    }


    void send_recv_border(grid& current_grid) const
    {

        if (top_rank != -1)
        {
            const auto border = current_grid.get_block_borders(direction::north);

            MPI_Send(border.data(), border.size(), MPI_DOUBLE, 
                top_rank, my_rank, communicator);
        }

        if (right_rank != -1)
        {
            const auto border = current_grid.get_block_borders(direction::east);

            MPI_Send(border.data(), border.size(), MPI_DOUBLE,
                right_rank, my_rank, communicator);
        }

        if (bottom_rank != -1)
        {
            const auto border = current_grid.get_block_borders(direction::south);

            MPI_Send(border.data(), border.size(), MPI_DOUBLE,
                bottom_rank, my_rank, communicator);
        }

        if (left_rank != -1)
        {
            const auto border = current_grid.get_block_borders(direction::west);

            MPI_Send(border.data(), border.size(), MPI_DOUBLE,
                left_rank, my_rank, communicator);
        }

        auto receive_buffer = std::vector<double>(current_grid.extent_x());

        Expects(current_grid.extent_x() == current_grid.extent_y());

        if (top_rank != -1)
        {
            MPI_Recv(receive_buffer.data(), receive_buffer.size(), MPI_DOUBLE, top_rank,
                     top_rank, communicator, MPI_STATUS_IGNORE);

            current_grid.set_block_borders(receive_buffer, direction::north);
        }

        if (right_rank != -1)
        {
            MPI_Recv(receive_buffer.data(), receive_buffer.size(), MPI_DOUBLE, right_rank,
                     right_rank, communicator, MPI_STATUS_IGNORE);

            current_grid.set_block_borders(receive_buffer, direction::east);
        }

        if (bottom_rank != -1)
        {
            MPI_Recv(receive_buffer.data(), receive_buffer.size(), MPI_DOUBLE, bottom_rank,
                     bottom_rank, communicator, MPI_STATUS_IGNORE);

            current_grid.set_block_borders(receive_buffer, direction::south);
        }

        if (left_rank != -1)
        {
            MPI_Recv(receive_buffer.data(), receive_buffer.size(), MPI_DOUBLE, left_rank,
                     left_rank, communicator, MPI_STATUS_IGNORE);

            current_grid.set_block_borders(receive_buffer, direction::west);
        }
    }

#if defined(TRIANGULAR)

    int execute(stencil &s, grid &grid1, grid &grid2)
    {
        int iteration=0;
        while (true)
        {
            s->iteration(grid1, grid2, 1);
            const auto d_epsilon = s->iteration(grid2, grid1, 0);
            s->send_recv_two_border(grid1);

            iteration+=2;

            double sum_epsilon;
            MPI_Allreduce(&d_epsilon, &sum_epsilon, 1, MPI_DOUBLE, MPI_SUM, communicator);

            if (sum_epsilon < epsilon)
                break;
        }
        return iteration;
    }

#else

    int execute(grid &grid1, grid &grid2) const
    {
        int iteration=0;
        while (true)
        {
            this->iteration(grid1, grid2);
            this->send_recv_border(grid1);
            const auto d_epsilon = this->iteration(grid2, grid1);
            this->send_recv_border(grid1);

            iteration+=2;

            double sum_epsilon;
            MPI_Allreduce(&d_epsilon, &sum_epsilon, 1, MPI_DOUBLE_PRECISION, MPI_SUM, communicator);

            if (sum_epsilon < epsilon)
                break;
        }
        return iteration;
    }

#endif
};



#endif //PARALLELSYSTEMS_MPI_STENCIL_H
