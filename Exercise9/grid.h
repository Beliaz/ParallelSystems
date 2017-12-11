//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_GRID_H
#define PARALLELSYSTEMS_GRID_H

#include <iostream>
#include <vector>
#include <cmath>
#include <gsl/gsl>

#if defined(N768)
constexpr auto n = 768;
#else
constexpr auto n = 512;
#endif
constexpr auto size = n * n;

MPI_Comm communicator;

enum class Direction
{
    north,
    east,
    south,
    west
};

class grid
{
public:
    explicit grid(
        const int my_rank,
        const int blocks,
        const gsl::span<const double, 4> borders)
        :   x_idx_(my_rank / blocks),
            y_idx_(my_rank % blocks),
            from_x_(x_idx_ * n / blocks),
            to_x_((x_idx_ + 1) * n / blocks),
            from_y_(y_idx_ * n / blocks),
            to_y_((y_idx_ + 1) * n / blocks),
            blocksize_(0),
            data_(size)
    {
        for (auto i = 0; i < n; i++)
        {
            data_[i] = borders[0];
            data_[size - 1 - i] = borders[2];
        }

        for (auto i = 1; i < n - 1; i++)
        {
            data_[n - 1 + i * n] = borders[1];
            data_[i * n] = borders[3];
        }
    }

    double& get(const unsigned int row, const unsigned int column)
    {
        Expects(row >= 0 && row < n);
        Expects(column >= 0 && column < n);

        return data_[linearize(row, column)];
    }

    const double& get(const unsigned int row, const unsigned int column) const
    {
        Expects(row >= 0 && row < n);
        Expects(column >= 0 && column < n);

        return data_[linearize(row, column)];
    }

    double get_five(const unsigned int row, const unsigned int column) const
    {
        Expects(row > 0 && row < n - 1);
        Expects(column > 0 && column < n - 1);

        return (get(row,column) +
                get(row - 1, column) +
                get(row + 1, column) +
                get(row, column - 1) +
                get(row, column + 1 ) ) / 5.0;
    }

    void set(const unsigned int row, const unsigned int column, const double value)
    {
        get(row, column) = value;
    }

    void print() 
    {
        for (long i = 0; i < n; i++) 
        {
            for (long j = 0; j < n; j++)
            {
                std::cout << data_[i * n + j] << " | ";
            }

            std::cout << std::endl;
        }
    }

    std::vector<double> get_block_borders(const Direction direction) const
    {
        std::vector<double> borders(blocksize_);

        for (auto i = 0u; i < blocksize_; ++i)
            borders[i] = get_border_element(direction, i);

        return borders;
    }

    void set_block_borders(const gsl::span<const double> borders,
                           const Direction direction)
    {
        for (auto i = 0u; i < blocksize_; ++i)
            get_border_element(direction, i) = borders[i];
    }

    size_t idx_x() const { return x_idx_; }
    size_t idx_y() const { return y_idx_; }

    size_t left_x() const { return from_x_; }
    size_t top_y() const { return from_y_; }

    size_t right_x() const { return to_x_; }
    size_t bottom_y() const { return to_y_; }

    size_t extent_x() const { return blocksize_; }
    size_t extent_y() const { return blocksize_; }

private:
    const size_t x_idx_;
    const size_t y_idx_;
    const size_t from_x_;
    const size_t to_x_;
    const size_t from_y_;
    const size_t to_y_;
    const size_t blocksize_;

    std::vector<double> data_;

    double& get_border_element(const Direction direction, const size_t idx)
    {
        switch (direction)
        {
        case Direction::north:  return get(from_x_ + idx, from_y_);
        case Direction::east:   return get(to_x_ - 1, from_y_ + idx);
        case Direction::south:  return get(from_x_ + idx, to_y_ - 1);
        case Direction::west:   return get(from_x_, from_y_ + idx);

        default: throw std::logic_error("invalid direction");
        }
    }

    const double& get_border_element(const Direction direction, const size_t element) const
    {
        return const_cast<grid&>(*this).get_border_element(direction, element);
    }

    static size_t linearize(const unsigned int row, const unsigned int column)
    {
        return column * n + row;
    }
};

#endif //PARALLELSYSTEMS_GRID_H
