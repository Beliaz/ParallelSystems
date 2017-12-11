//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_GRID_H
#define PARALLELSYSTEMS_GRID_H

#include <iostream>
#include <vector>
#include <cmath>
#include <gsl/gsl>

constexpr auto n = 512;
constexpr auto size = n * n;



class grid
{
public:

    unsigned int xpos;
    unsigned int ypos;
    unsigned int from_x;
    unsigned int to_x;
    unsigned int from_y;
    unsigned int to_y;
    unsigned int blocksize;


    void set_from_to(
        const unsigned int from_x, 
        const unsigned int to_x, 
        const unsigned int from_y, 
        const unsigned int to_y)
    {
        this->from_x=from_x;
        this->to_x=to_x;
        this->from_y=from_y;
        this->to_y=to_y;
        this->blocksize=to_x-from_x;
    }

    explicit grid(const gsl::span<const double, 4> borders) 
        :   xpos(0), 
            ypos(0), 
            from_x(0), 
            to_x(0), 
            from_y(0), 
            to_y(0), 
            blocksize(0),
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

    double get(const unsigned int row, const unsigned int column) const
    {
        Expects(row >= 0 && row < n);
        Expects(column >= 0 && column < n);

        return data_[row * n + column];
    }

    double get_five(const unsigned int row, const unsigned int column) const
    {
        Expects(row >= 0 && row < n);
        Expects(column >= 0 && column < n);

        return (get(row,column) +
                get(row - 1, column) +
                get(row + 1, column) +
                get(row, column - 1) +
                get(row, column +1 ) ) / 5.0;
    }

    void set(const unsigned int row, const unsigned int column, const double value)
    {
        Expects(row >= 0 && row < n);
        Expects(column >= 0 && column < n);

        data_[row * n + column] = value;
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

    std::vector<std::vector<double>> get_block_borders() const
    {
        const auto size = to_x - from_x;

        auto borders = std::vector<std::vector<double>>();

        borders.push_back(std::vector<double>(size));
        borders.push_back(std::vector<double>(size));
        borders.push_back(std::vector<double>(size));
        borders.push_back(std::vector<double>(size));

        for (auto i = 0u; i < size; ++i) {
            borders[0][i] = get(from_x , from_y + i);
            //      borders[0][size + counter++] = get(from_x + 1, from_y + i);
        }

        for (auto i = 0u; i < size; ++i) {
            borders[1][i] = get(from_x + i, to_y);
            //     borders[1][size + counter++] = get(from_x + i, to_y - 1);
        }

        for (auto i = 0u; i < size; ++i) {
            borders[2][i] = get(to_x, from_y + i);
            //    borders[2][size + counter++] = get(to_x - 1, from_y + i);
        }

        for (auto i = 0u; i < size; ++i) {
            borders[3][i] = get(from_x + i, from_y);
            //   borders[3][size + counter++] = get(from_x + i, from_y + 1);
        }

        return borders;
    }

    void set_block_borders(const gsl::span<const double> borders,
                           const unsigned direction)
    {
        const auto size = to_x - from_x;

        switch (direction)
        {
            case 0:
            {
                for (auto i = 0u; i < size; ++i)
                    set(from_x - 1, from_y + i, borders[i]);

            } break;

            case 1:
            {
                for (auto i = 0u; i < size; ++i)
                    set(from_x + i, to_y + 1, borders[i]);

            } break;

            case 2:
            {
                for (auto i = 0u; i < size; ++i)
                    set(to_x + 1, from_y + i, borders[i]);

            } break;

            case 3:
            {
                for (auto i = 0u; i < size; ++i)
                    set(from_x + i, from_y - 1, borders[i]);

            } break;

            default: throw std::logic_error("unexpected direction");
        }
    }

    void set_block(const int my_rank, const int blocks)
    {
        xpos = my_rank / blocks;
        ypos = my_rank % blocks;

        const auto elems_per_block = n / blocks;

        set_from_to(
            xpos * elems_per_block, 
            (xpos + 1) * elems_per_block, 
            ypos * elems_per_block, 
            (ypos + 1) * elems_per_block);
    }

private:
    std::vector<double> data_;
};

#endif //PARALLELSYSTEMS_GRID_H
