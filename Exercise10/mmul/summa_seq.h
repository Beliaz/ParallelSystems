#ifndef SUMMA_SEQ_H
#define SUMMA_SEQ_H

#include "matrix.h"
#include "gsl/gsl"
#include <iostream>
#include "naive_seq.h"
#include <iomanip>

struct summa_sequential { };

template<typename T>
using block_t = std::array<matrix<T>, 3>;

template<typename T>
using block_grid_t = std::vector<std::vector<block_t<T>>>;

template<typename T>
matrix<T> compute_outer_product(
    const matrix<T>& a,
    const typename matrix<T>::size_type column_index,
    const matrix<T>& b,
    const typename matrix<T>::size_type row_index);

template<typename T>
void print_blocks(block_grid_t<T> blocks);

template<typename T>
void update_blocks(std::vector<std::vector<std::array<T, 3>>>& blocks, const size_t k);

template<typename T>
void update_block(std::array<matrix<T>, 3>& block, size_t k);

template<typename T>
void copy_col(matrix<T>& dest, const matrix<T>& src, size_t column);

template<typename T>
void copy_row(matrix<T>& dest, const matrix<T>& src, size_t row);

template<typename T>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, summa_sequential)
{
    Expects(a.n() == b.n());

    const auto n = a.n();

    const auto blocks_per_dim = get_num_blocks_per_dim(n, 16);

    if (blocks_per_dim == 1)
    {
        return multiply(a, b, naive_sequential());
    }

    const auto block_size = n / blocks_per_dim;

    std::vector<std::vector<std::array<matrix_t, 3>>> blocks;

    blocks.resize(blocks_per_dim);
    for (auto& row : blocks)
    {
        row.resize(blocks_per_dim,
        {
            matrix_t(block_size),
            matrix_t(block_size),
            matrix_t(block_size, 0)
        });
    }

    for (long i = 0; i < n; ++i)
    {
        for (long j = 0; j < n; ++j)
        {
            const auto block_y = i / block_size;
            const auto block_x = j / block_size;

            auto& block = blocks[block_y][block_x];

            const auto block_offset_x = block_x * block_size;
            const auto local_x = j - block_offset_x;

            const auto block_offset_y = block_y * block_size;
            const auto local_y = i - block_offset_y;

            block[0](local_y, local_x) = a(i, j);
            block[1](local_y, local_x) = b(i, j);
        }
    }

    print_blocks(blocks);

    auto old_blocks = blocks;

    for (auto y = gsl::narrow<long>(blocks_per_dim - 1); y >= 0; --y)
    {
        for (auto x = gsl::narrow<long>(blocks_per_dim - 1); x >= 0; --x)
        {
            const auto src_a_y = y % blocks_per_dim;
            const auto src_a_x = (-x - y) % blocks_per_dim;

            const auto src_b_y = (-x - y) % blocks_per_dim;
            const auto src_b_x = x % blocks_per_dim;

            blocks[y][x][0] = old_blocks[src_a_y][src_a_x][0];
            blocks[y][x][1] = old_blocks[src_b_y][src_b_x][1];
        }
    }

    print_blocks(blocks);

    for (size_t k = 0; k < a.n(); ++k)
    {
        update_blocks(blocks, k % block_size);

        old_blocks = blocks;

        for (auto y = gsl::narrow<long>(blocks_per_dim - 1); y >= 0; --y)
        {
            for (auto x = gsl::narrow<long>(blocks_per_dim - 1); x >= 0; --x)
            {
                auto& block = blocks[y][x];
                auto& block_a = block[0];
                auto& block_b = block[1];

                const auto neighbour_x = x == 0
                    ? blocks_per_dim - 1
                    : x - 1;

                const auto neighbour_y = y == 0
                    ? blocks_per_dim - 1
                    : y - 1;

                copy_row(block_b, old_blocks[neighbour_y][x][1], k % block_size);
                copy_col(block_a, old_blocks[y][neighbour_x][0], k % block_size);
            }
        }

        print_blocks(blocks);
    }

    auto c = matrix_t(n);

    for (size_t i = 0; i < n; i += block_size)
    {
        for (size_t j = 0; j < n; j += block_size)
        {
            const auto block_i = i / block_size;
            const auto block_j = j / block_size;

            const auto block_offset_i = block_i * block_size;
            const auto block_offset_j = block_j * block_size;

            auto& block = blocks[block_i][block_j];

            const auto local_i = i - block_offset_i;
            const auto local_j = j - block_offset_j;

            c(i, j) = block[2](local_i, local_j);
        }
    }

    return c;
}

inline size_t get_num_blocks_per_dim(const size_t n, const size_t num_procs)
{
    Expects(n > 0);
    Expects(num_procs > 0);

    if (num_procs == 1) return 1;

    const auto procs_per_dim = gsl::narrow<size_t>(
        std::floor(std::sqrt(num_procs)));

    const auto num_procs_used = procs_per_dim * procs_per_dim;

    if (num_procs_used != num_procs)
    {
        std::cout << "warning only using "
            << num_procs_used << " out of "
            << num_procs << " processors " << std::endl;

    }

    if (n % procs_per_dim != 0)
    {
        std::cout << "error: n (" << n
            << ") not divisible by processor count ("
            << procs_per_dim << ")" << std::endl;

        throw;
    }

    return procs_per_dim;
}

template<typename T>
matrix<T> compute_outer_product(
    const matrix<T>& a,
    const typename matrix<T>::size_type column_index,
    const matrix<T>& b,
    const typename matrix<T>::size_type row_index)
{
    Expects(a.n() == b.n());

    matrix<T> c(a.n());

    for (size_t i = 0; i < a.n(); ++i)
    {
        for (size_t j = 0; j < b.n(); ++j)
        {
            c(i, j) = a(i, column_index) * b(row_index, j);
        }
    }

    return c;
}

template<typename T>
void print_blocks(std::vector<std::vector<std::array<matrix<T>, 3>>> blocks)
{
    const auto block_size = blocks[0][0][0].n();
    const auto num_blocks = blocks.size();
    const auto n = block_size * num_blocks;

    for (size_t i = 0; i < n; ++i)
    {
        const auto block_i = i / block_size;

        const auto block_offset_i = block_i * block_size;

        const auto local_i = i - block_offset_i;

        if (i > 0 && i % block_size == 0)
        {
            for (size_t m = 0; m < 3; ++m)
            {
                std::cout << std::setfill('-') << std::setw(n * 3 + num_blocks - 1) << "-";
                std::cout << "    ";
            }

            std::cout << std::endl;
        }

        for (size_t m = 0; m < 3; ++m)
        {
            for (size_t block_j = 0; block_j < num_blocks; ++block_j)
            {
                const auto& block = blocks[block_i][block_j];
                const auto& mat = block[m];

                for (size_t j = 0; j < block_size; ++j)
                {
                    std::cout << std::setfill(' ') << std::setw(2) << mat(local_i, j) << " ";
                }

                if (block_j + 1 == num_blocks) continue;

                std::cout << "|";
            }

            std::cout << "    ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

template<typename T>
void copy_row(matrix<T>& dest, const matrix<T>& src, size_t row)
{
    for (size_t m = 0; m < dest.n(); ++m)
    {
        dest(row, m) = src(row, m);
    }
}

template<typename T>
void copy_col(matrix<T>& dest, const matrix<T>& src, size_t column)
{
    for (size_t m = 0; m < dest.n(); ++m)
    {
        dest(m, column) = src(m, column);
    }
}

template<typename T>
void update_block(std::array<matrix<T>, 3>& block, size_t k)
{
    const auto& a = block[0];
    const auto& b = block[1];
    auto& c = block[2];

    c += compute_outer_product(a, k, b, k);
}

template<typename T>
void update_blocks(std::vector<std::vector<std::array<T, 3>>>& blocks, const size_t k)
{
    for (auto& block_row : blocks)
    {
        for (auto& block : block_row)
        {
            update_block(block, k);
        }
    }
}



#endif // SUMMA_SEQ_H
