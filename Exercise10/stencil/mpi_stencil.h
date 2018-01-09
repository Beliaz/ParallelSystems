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

        const auto left_top_rank        = index(grid1.idx_x() - 1, grid1.idx_y() - 1);
        const auto top_rank             = index(grid1.idx_x(), grid1.idx_y() - 1);
        const auto right_top_rank       = index(grid1.idx_x() + 1, grid1.idx_y() - 1);
        const auto left_rank            = index(grid1.idx_x() - 1, grid1.idx_y());
        const auto right_rank           = index(grid1.idx_x() + 1, grid1.idx_y());
        const auto left_bottom_rank     = index(grid1.idx_x() - 1, grid1.idx_y() + 1);
        const auto bottom_rank          = index(grid1.idx_x(), grid1.idx_y() + 1);
        const auto right_bottom_rank    = index(grid1.idx_x() + 1, grid1.idx_y() + 1);

        if (grid1.idx_y() > 0)
            neighbours.push_back(neighbour_t(direction_t::north, top_rank));

        if (grid1.idx_y() > 0 && grid1.idx_x() > 0)
            neighbours.push_back(neighbour_t(direction_t::north, left_top_rank));

        if (grid1.idx_y() > 0 && grid1.idx_x() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction_t::north, right_top_rank));

        if (grid1.idx_y() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction_t::south, bottom_rank));

        if (grid1.idx_y() < num_blocks - 1 && grid1.idx_x() > 0)
            neighbours.push_back(neighbour_t(direction_t::south, left_bottom_rank));

        if (grid1.idx_y() < num_blocks - 1 && grid1.idx_x() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction_t::south, right_bottom_rank));

        if (grid1.idx_x() > 0)
            neighbours.push_back(neighbour_t(direction_t::west, left_rank));

        if (grid1.idx_x() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction_t::east, right_rank));

    }

    int index(int x, int y)
    {
        return y * this->num_blocks + x;
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


    ////////////////////////////////////////INNER BORDER THING SLOW..
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

    //////////////////////////////////////// INNER BORDER THING END


    //////////////////////////////////////// NEW OPTIMIZION


    double optimized_iteration_v2(const grid& source, grid& target) const
    {
        auto epsilon = full_iteration(source, target);


        return epsilon;
    }

    bool higher(const grid& g1, const grid& g2) const {
        int middle = g1.bottom_y() - g1.top_y();
        if(g1.get(g1.top_y() + middle, g1.left_x() + middle) > g2.get(g2.top_y() + middle, g2.left_x() + middle))
            return true;
        else
            return false;
    }

    double full_iteration(const grid& source, grid& target) const
    {
        pyramid(source,target,elements_per_block,0);
        if(higher(source,target)) {
            send_data(source,iterations);
            pyramid(source,target,0,-elements_per_block/2);
        }else {
            send_data(target,iterations);
            pyramid(target,source,0,-elements_per_block/2);
        }
        if(higher(source,target))
            receive_data(source);
        else
            receive_data(target);

        auto epsilon = epsilon_iteration(source,target);
        return epsilon;
    }

    void pyramid(const grid& source, grid& target, int offset, int quit_cond) const
    {
        if(offset == quit_cond)
            return;

        for (auto row = source.top_y() - offset; row < source.bottom_y() + offset; row++)
        {
            if (row < 0)     continue;
            if (row > n - 1) continue;

            for (auto column = source.left_x() - offset; column < source.right_x() + offset; column++)
            {
                if (column == 0)     continue;
                if (column > n - 1) continue;

                const auto current = source.get(row , column);
                const auto new_value = source.get_five(row , column);

                target.set(row , column , new_value);
                target.incr_iteration(row,column);
            }
        }

        pyramid(target,source,offset-1);
    }

    double inner_iteration(const grid& source, grid& target) const
    {

    }

    void send_data(const grid& current_grid) const {
        const auto own_data = current_grid.own_data();
        for(const auto& neighbour : neighbours)
        {
            const auto direction = std::get<0>(neighbour);
            const auto rank = std::get<1>(neighbour);

            MPI_Send(own_data.data(), own_data.size(), MPI_DOUBLE,
                     rank, my_rank, communicator);
        }
    }

    void receive_data(const grid& current_grid) const {
        auto receive_buffer = std::vector<double>(current_grid.extent_x()*current_grid.extent_y());
        for (const auto& neighbour : neighbours)
        {
            const auto direction = std::get<0>(neighbour);
            const auto rank = std::get<1>(neighbour);

            MPI_Recv(receive_buffer.data(), receive_buffer.size(), MPI_DOUBLE, rank,
                     rank, communicator, MPI_STATUS_IGNORE);

            current_grid.set_other_data(receive_buffer, direction);
        }
    }













    //////////////////////////////////////// END NEW OPTIMIZION


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
        unsigned int mini_batch_size = 1000 / epsilon;

        auto iterations = 0;

        while (true)
        {
            for(unsigned int i = 0; i < mini_batch_size; i++)
            {
                iteration(grid1, grid2);
                iteration(grid2, grid1);
            }

            iteration(grid1, grid2);

            const auto actual_epsilon = iteration(grid2, grid1);

            iterations += 2 + mini_batch_size * 2;;

            double sum_epsilon;
            MPI_Allreduce(&actual_epsilon, &sum_epsilon, 1, MPI_DOUBLE, MPI_SUM, communicator);

            if (sum_epsilon < epsilon)
                break;

            mini_batch_size = sum_epsilon / epsilon * 10;
        }

        return iterations;
    }
};



#endif //PARALLELSYSTEMS_MPI_STENCIL_H
