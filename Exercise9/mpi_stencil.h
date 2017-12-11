//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_MPI_STENCIL_H
#define PARALLELSYSTEMS_MPI_STENCIL_H

#include "grid.h"
#include <memory>

constexpr auto epsilon = 20;


class stencil
{
public:
    const int my_rank;
    int top_rank;
    int right_rank;
    int bottom_rank;
    int left_rank;

    using neighbour_t = std::tuple<direction, int>;
    std::vector<neighbour_t> neighbours;

    const size_t num_blocks;
    const size_t elements_per_block;

    stencil(const int num_procs, const int my_rank, const grid& grid1) :
        my_rank(my_rank), 
        num_blocks(static_cast<int>(sqrt(num_procs))), 
        elements_per_block(n / num_blocks)
    {

        left_rank = grid1.idx_y() * num_blocks + grid1.idx_x() - 1;
        right_rank = grid1.idx_y() * num_blocks + grid1.idx_x() + 1;

        top_rank = (grid1.idx_y() - 1) * num_blocks + grid1.idx_x();
        bottom_rank = (grid1.idx_y() + 1) * num_blocks + grid1.idx_x();

        if (grid1.idx_x() > 0u)          
            neighbours.push_back(neighbour_t(direction::east, left_rank));

        if (grid1.idx_x() < num_blocks - 1) 
            neighbours.push_back(neighbour_t(direction::west, right_rank));

        if(grid1.idx_y() > 0u)
            neighbours.push_back(neighbour_t(direction::north, top_rank));

        if(grid1.idx_y() < num_blocks - 1)
            neighbours.push_back(neighbour_t(direction::south, bottom_rank));
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


    void send_borders(grid& current_grid) const
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
            MPI_Allreduce(&d_epsilon, &sum_epsilon, 1, MPI_DOUBLE, MPI_SUM, communicator);

            if (sum_epsilon < epsilon)
                break;
        }
        return iteration;
    }

#endif
};



#endif //PARALLELSYSTEMS_MPI_STENCIL_H
