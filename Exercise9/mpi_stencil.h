//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_MPI_STENCIL_H
#define PARALLELSYSTEMS_MPI_STENCIL_H

#include "grid.h"
#include <mpi.h>
#include <iostream>

class stencil
{
public:
    const int my_rank;

    using neighbour_t = std::tuple<direction_t, int>;
    std::vector<neighbour_t> neighbours;

    const size_t num_blocks;
    const size_t elements_per_block;

    stencil(const int num_procs, const int my_rank, const grid& grid1) :
        my_rank(my_rank),
        num_blocks(static_cast<int>(sqrt(num_procs))),
        elements_per_block(n / num_blocks)
    {

        /*if (my_rank == 0)
        {
            std::cout << "num_blocks: " << num_blocks << std::endl;
            std::cout << "elements_per_block: " << elements_per_block << std::endl;
            std::cout << std::endl;
        }

        std::cout   << "my_rank: " << my_rank << std::endl
                    << "idx_x: " << grid1.idx_x() << ", idx_y: " << grid1.idx_y() << std::endl
                    << "from x: " << grid1.left_x() << ", to_x: " << grid1.right_x() << std::endl
                    << "from y: " << grid1.top_y() << ", to_y: " << grid1.bottom_y() << std::endl 
                    << "neighbours: ";
        */

        const auto left_rank = grid1.idx_y() * num_blocks + grid1.idx_x() - 1;
        const auto right_rank = grid1.idx_y() * num_blocks + grid1.idx_x() + 1;

        const auto top_rank = (grid1.idx_y() - 1) * num_blocks + grid1.idx_x();
        const auto bottom_rank = (grid1.idx_y() + 1) * num_blocks + grid1.idx_x();

        if (grid1.idx_y() > 0)
            neighbours.push_back(neighbour_t(direction_t::north, top_rank));

        if (grid1.idx_y() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction_t::south, bottom_rank));

        if (grid1.idx_x() > 0)
            neighbours.push_back(neighbour_t(direction_t::west, left_rank));

        if (grid1.idx_x() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction_t::east, right_rank));

        /*for (const auto &neighbour : neighbours) 
        {
            const auto direction = std::get<0>(neighbour);
            const auto rank = std::get<1>(neighbour);

            switch (direction)
            {
            case direction_t::north:    std::cout << "north"; break;
            case direction_t::east:     std::cout << "east";  break;
            case direction_t::south:    std::cout << "south"; break;
            case direction_t::west:     std::cout << "west";  break;
            default: throw "invalid direction";
            }

            std::cout << "(" << rank << "), ";
        }

        std::cout << std::endl;
        std::cout << std::endl;
        */
    }


    //Does the job and returns the delta Epsilon accumulated over all comparisons.
    static double iteration(const grid& source, grid& target) 
    {
        auto epsilon = 0.0;

        for (auto row = source.top_y(); row < source.bottom_y(); row++) 
        {
            if (row == 0)     continue;
            if (row == n - 1) continue;

            for (auto column = source.left_x(); column < source.right_x(); column++) 
            {
                if (column == 0)     continue;
                if (column == n - 1) continue;
                  
                const auto current = source.get(row , column);
                const auto new_value = source.get_five(row , column);

                target.set(row , column , new_value);

                epsilon += std::abs(new_value - current);
            }
        }

        return epsilon;
    }

    static double inner_border_iteration(const grid& source, grid& target)
    {
        auto epsilon = 0.0;

        for (auto row = source.top_y(); row < source.bottom_y(); row++)
        {
            if (row == 0)     continue;
            if (row == n - 1) continue;

            if (row == source.top_y() || row == source.bottom_y())
            {
                for (auto column = source.left_x(); column < source.right_x(); column++)
                {
                    if (column == 0)     continue;
                    if (column == n - 1) continue;

                    const auto current = source.get(row, column);
                    const auto new_value = source.get_five(row, column);

                    target.set(row, column, new_value);

                    epsilon += std::abs(new_value - current);
                }

                continue;
            }


            auto column = source.left_x();

            if(column > 0 && column < n - 1)
            {
                const auto current = source.get(row, column);
                const auto new_value = source.get_five(row, column);

                target.set(row, column, new_value);

                epsilon += std::abs(new_value - current);
            }

            column = source.right_x();

            if (column > 0 && column < n - 1)
            {
                const auto current = source.get(row, column);
                const auto new_value = source.get_five(row, column);

                target.set(row, column, new_value);

                epsilon += std::abs(new_value - current);
            }
        }

        return epsilon;
    }

    static double inner_iteration(const grid& source, grid& target)
    {
        auto epsilon = 0.0;

        for (auto row = source.top_y() + 1; row < source.bottom_y() - 1; row++)
        {
            for (auto column = source.left_x() + 1; column < source.right_x() - 1; column++)
            {
                const auto current = source.get(row, column);
                const auto new_value = source.get_five(row, column);

                target.set(row, column, new_value);

                epsilon += std::abs(new_value - current);
            }
        }

        return epsilon;
    }

    double optimized_iteration(const grid& source, grid& target) const
    {
        auto epsilon = inner_border_iteration(source, target);

        send_borders(target);

        epsilon += inner_iteration(source, target);

        receive_borders(target);

        return epsilon;
    }


    void send_borders(const grid& current_grid) const
    {
        for(const auto& neighbour : neighbours)
        {
            const auto direction = std::get<0>(neighbour);
            const auto rank = std::get<1>(neighbour);

            const auto border = current_grid.get_block_borders(direction);

            MPI_Send(border.data(), border.size(), MPI_DOUBLE, 
                rank, my_rank, communicator);
        }
    }

    void receive_borders(grid& current_grid) const
    {
        Expects(current_grid.extent_x() == current_grid.extent_y());

        auto receive_buffer = std::vector<double>(current_grid.extent_x());

        for (const auto& neighbour : neighbours)
        {
            const auto direction = std::get<0>(neighbour);
            const auto rank = std::get<1>(neighbour);

            MPI_Recv(receive_buffer.data(), receive_buffer.size(), MPI_DOUBLE, rank,
                     rank, communicator, MPI_STATUS_IGNORE);

            current_grid.set_block_borders(receive_buffer, direction);
        }
    }

    void send_recv_border(grid& current_grid) const
    {
        send_borders(current_grid);
        receive_borders(current_grid);
    }

    int execute(grid &grid1, grid &grid2) const
    {
        auto iterations = 0;

        while (true)
        {
            optimized_iteration(grid1, grid2);

            const auto d_epsilon = optimized_iteration(grid2, grid1);

            iterations += 2;

            double sum_epsilon;
            MPI_Allreduce(&d_epsilon, &sum_epsilon, 1, MPI_DOUBLE, MPI_SUM, communicator);

            if (sum_epsilon < epsilon)
                break;
        }

        return iterations;
    }
};



#endif //PARALLELSYSTEMS_MPI_STENCIL_H
