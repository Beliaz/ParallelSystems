//
// Created by ivan on 06/12/17.
//

#ifndef PARALLELSYSTEMS_MPI_STENCIL_H
#define PARALLELSYSTEMS_MPI_STENCIL_H

#include "grid.h"
#include <memory>

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

    stencil(const int num_procs, const int my_rank, grid* grid1) :
        my_rank(my_rank), 
        num_blocks(static_cast<int>(sqrt(num_procs))), 
        elements_per_block(n / num_blocks)
    {

        left_rank = grid1->xpos * num_blocks + grid1->ypos - 1;
        right_rank = grid1->xpos * num_blocks + grid1->ypos + 1;
        top_rank = (grid1->xpos - 1) * num_blocks + grid1->ypos;
        bottom_rank = (grid1->xpos + 1) * num_blocks + grid1->ypos;

        if (grid1->xpos == 0u)
        {
            top_rank = -1;
        }
        else if (static_cast<int>(grid1->xpos) == num_blocks - 1)
        {
            bottom_rank = -1;
        }

        if(grid1->ypos == 0u)
        {
            left_rank = -1;
        }
        else if(static_cast<int>(grid1->ypos) == num_blocks - 1)
        {
            right_rank = -1;
        }
    }


    //Does the job and returns the delta Epsilon accumulated over all comparisons.
    static double iteration(grid *source, grid *target) 
    {
        auto d_epsilon = 0.0;

        for (auto i = source->from_x; i < source->to_x; i++) 
        {
            if (i == 0)     continue;
            if (i == n - 1) continue;

            for (auto j = source->from_y; j < source->to_y; j++) 
            {
                if (j == 0)     continue;
                if (j == n - 1) continue;
                  
                const auto current = source->get(i , j);
                const auto new_value = source->get_five(i , j);

                target->set(i , j , new_value);

                d_epsilon += current - new_value > 0
                    ? current - new_value
                    : new_value - current;
            }
        }

        return d_epsilon;
    }


    void send_recv_border(grid *current_grid)
    {
        // SEND if neighbor exist
        const auto send = current_grid->get_block_borders();

        if (top_rank != -1)
        {
            MPI_Send(send[0].data(), elements_per_block, MPI_DOUBLE, top_rank,
                my_rank, MPI_COMM_WORLD);
        }

        if (right_rank != -1)
        {
            MPI_Send(send[1].data(), elements_per_block, MPI_DOUBLE, right_rank,
                my_rank, MPI_COMM_WORLD);
        }

        if (bottom_rank != -1)
        {
            MPI_Send(send[2].data(), elements_per_block, MPI_DOUBLE,
                bottom_rank, my_rank, MPI_COMM_WORLD);
        }

        if (left_rank != -1)
        {
            MPI_Send(send[3].data(), elements_per_block, MPI_DOUBLE, left_rank,
                my_rank, MPI_COMM_WORLD);
        }


        // receive if neighbor exist
        auto recv = std::make_unique<double[]>(current_grid->to_x - current_grid->from_x);

        if (top_rank != -1) 
        {
            MPI_Recv(recv.get(), elements_per_block, MPI_DOUBLE, top_rank,
                     top_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            current_grid->set_block_borders(recv.get(), 0);
        }

        if (right_rank != -1)
        {
            MPI_Recv(recv.get(), elements_per_block, MPI_DOUBLE, right_rank,
                     right_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            current_grid->set_block_borders(recv.get(), 1);
        }

        if (bottom_rank != -1) 
        {
            MPI_Recv(recv.get(), elements_per_block, MPI_DOUBLE, bottom_rank,
                     bottom_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            current_grid->set_block_borders(recv.get(), 2);
        }

        if (left_rank != -1) 
        {
            MPI_Recv(recv.get(), elements_per_block, MPI_DOUBLE, left_rank,
                     left_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            current_grid->set_block_borders(recv.get(), 3);
        }
    }
};



#endif //PARALLELSYSTEMS_MPI_STENCIL_H
