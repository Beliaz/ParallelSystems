//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_GRID_H
#define PARALLELSYSTEMS_GRID_H

#include <iostream>
#include <vector>
#include <cmath>
#include <gsl/gsl>
#include <mpi.h>

#if defined(N768)
constexpr auto n = 768;
#else
constexpr auto n = 512;
#endif

constexpr auto epsilon = 1.0;

constexpr auto size = n * n;

MPI_Comm communicator;

enum class direction_t
{
    north,
    east,
    south,
    west,
    northeast,
    northwest,
    southeast,
    southwest
};

class grid
{
public:
    explicit grid(
        const int my_rank,
        const int blocks,
        const gsl::span<const double, 4> borders)
        :   blocksize_(n / blocks),
            x_idx_(my_rank % blocks),
            y_idx_(my_rank / blocks),
            from_x_(x_idx_ * blocksize_),
            to_x_((x_idx_ + 1) * blocksize_),
            from_y_(y_idx_ * blocksize_),
            to_y_((y_idx_ + 1) * blocksize_),
    
            data_(size),
            iteration_high(size)
    {
        for (auto i = 0; i < n ; i++)
            for (auto j = 0; j < n ; j++)
                iteration_high[linearize(i,j)]=0;


        for (auto i = 0; i < n; i++)
        {
            set(0, i, borders[0]);
            set(n - 1, i, borders[2]);
        }

        for (auto i = 1; i < n - 1; i++)
        {
            set(i, 0, borders[1]);
            set(i, n - 1, borders[3]);
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

        return (get(row, column) +
                get(row - 1, column) +
                get(row + 1, column) +
                get(row, column - 1) +
                get(row, column + 1 ) ) / 5.0;
    }

    void set(const unsigned int row, const unsigned int column, const double value)
    {
        get(row, column) = value;
    }

    void incr_iteration(const unsigned int row, const unsigned int column)
    {
        iteration_high[linearize(row,column)]++;
    }

    void set_iteration(const unsigned int row, const unsigned int column, const int value)
    {
        iteration_high[linearize(row,column)] = value;
    }

    int get_iteration(const unsigned int row, const unsigned int column)
    {
        return iteration_high[linearize(row,column)];
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

    std::vector<double> get_block_borders(const direction_t direction) const
    {

        std::vector<double> borders(blocksize_);

        for (auto i = 0u; i < blocksize_; ++i)
            borders[i] = get_inner_border_element(direction, i);

        return borders;
    }

    void set_block_borders(const gsl::span<const double> borders,
                           const direction_t direction)
    {
        for (auto i = 0u; i < blocksize_; ++i)
            get_outer_border_element(direction, i) = borders[i];
    }

    std::vector<double> own_data() const
    {
        std::vector<double> retval(blocksize_*blocksize_);
        for (auto row = top_y(); row < bottom_y(); ++row) {
            for (auto column = left_x(); column < right_x(); ++column) {
                retval[linearize(row,column)] = get(row,column);
            }
        }
        return retval;
    }

    void set_other_data(const std::vector<double> other_data,
                        const direction_t direction){
        int min_row = top_y();
        int max_row = bottom_y();
        int min_column = left_x();
        int max_column = right_x();
        switch (direction){
            case direction_t ::north:
                min_row = top_y() - blocksize_;
                max_row = top_y();
                break;
            case direction_t ::east:
                min_column = right_x();
                max_column = right_x() + blocksize_;
                break;
            case direction_t ::south:
                min_row = bottom_y();
                max_row = bottom_y() + blocksize_;
                break;
            case direction_t ::west:
                min_column = left_x() - blocksize_;
                max_column = left_x();
                break;
            case direction_t ::northeast:
                min_row = top_y() - blocksize_;
                max_row = top_y();
                min_column = right_x();
                max_column = right_x() + blocksize_;
                break;
            case direction_t ::northwest:
                min_column = left_x() - blocksize_;
                max_column = left_x();
                min_row = top_y() - blocksize_;
                max_row = top_y();
                break;
            case direction_t ::southeast:
                min_row = bottom_y();
                max_row = bottom_y() + blocksize_;
                min_column = right_x();
                max_column = right_x() + blocksize_;
                break;
            case direction_t ::southwest:
                min_row = bottom_y();
                max_row = bottom_y() + blocksize_;
                min_column = left_x() - blocksize_;
                max_column = left_x();
                break;
        }
        int counter=0;
        for (int row = min_row; row < max_row; ++row) {
            for (int column = min_column; column < max_column; ++column) {
                data_[linearize(row,column)] = other_data[counter++];
            }
        }
    }

    size_t idx_y() const { return y_idx_; }
    size_t idx_x() const { return x_idx_; }

    size_t left_x() const { return from_x_; }
    size_t top_y() const { return from_y_; }

    size_t right_x() const { return to_x_; }
    size_t bottom_y() const { return to_y_; }

    size_t extent_x() const { return blocksize_; }
    size_t extent_y() const { return blocksize_; }

private:
    const size_t blocksize_;
    const size_t x_idx_;
    const size_t y_idx_;
    const size_t from_x_;
    const size_t to_x_;
    const size_t from_y_;
    const size_t to_y_;

    std::vector<double> data_;
    std::vector<int> iteration_high;

    double& get_border_element(const direction_t direction, const size_t idx, const size_t offset)
    {
        switch (direction)
        {
        case direction_t::north:  return get(from_y_ - offset, from_x_ + idx);
        case direction_t::east:   return get(from_y_ + idx, to_x_ - 1 + offset);
        case direction_t::south:  return get(to_y_ - 1 + offset, from_x_ + idx);
        case direction_t::west:   return get(from_y_ + idx , from_x_ - offset);

        default: throw std::logic_error("invalid direction");
        }
    }

    const double& get_inner_border_element(const direction_t direction, const size_t element) const
    {
        return const_cast<grid&>(*this).get_border_element(direction, element, 0);
    }

    double& get_outer_border_element(const direction_t direction, const size_t element)
    {
        return get_border_element(direction, element, 1);
    }

    static size_t linearize(const unsigned int row, const unsigned int column)
    {
        return row * n + column;
    }
};

#endif //PARALLELSYSTEMS_GRID_H
