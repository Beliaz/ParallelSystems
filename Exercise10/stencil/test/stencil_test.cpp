//
// Created by ivan on 11/12/17.
//

#include <cmath>
#include <string>
#include <iostream>
#include <chrono>

#define N512

#include "../mpi_stencil.h"
#include "sequential/jacobi.h"
#include "sequential/iteration.h"
#include "sequential/stencil_code.h"
#include "sequential/print.h"

#include <mpi.h>

using stencil_code_t = stencil_code::stencil_iteration<stencil_code::jacobi>;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    auto num_procs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    auto blocks = static_cast<int>(sqrt(num_procs));

    if (blocks * blocks != num_procs)
    {
        if (my_rank >= blocks * blocks)
        {
            MPI_Comm_split(MPI_COMM_WORLD, 1, my_rank, &communicator);
            MPI_Finalize();

            throw std::runtime_error("invalid rank");
        }

        MPI_Comm_split(MPI_COMM_WORLD, 0, my_rank, &communicator);
    }
    else
    {
        communicator = MPI_COMM_WORLD;
    }

    MPI_Comm_size(communicator, &num_procs);
    MPI_Comm_rank(communicator, &my_rank);

    blocks = static_cast<int>(sqrt(num_procs));

    const std::array<const double, 4> borders
    {
            1.0, 0.5, 0, -0.5
    };

    grid grid1(my_rank, blocks, borders);
    grid grid2(my_rank, blocks, borders);

    const ::stencil s(num_procs, my_rank, grid1);

    const auto iterations = s.execute(grid1, grid2);

    MPI_Finalize();

    if (my_rank != 0) return EXIT_SUCCESS;

    // sequential

    constexpr ::bounds_t<2> bounds = { -0.5, 0.5, 1.0, 0 };
    const stencil_code::grid_extents_t<2> extents = { n, n };

    auto grid_old = create_buffered_grid(extents, bounds, default_value);
    auto iterations_old = 0;

    for(;;)
    {
        const auto result = stencil_code_t::do_iteration(grid_old, epsilon);

        iterations_old += std::get<0>(result);

        if (std::get<1>(result)) break;
    }

    const auto iteration_diff = std::abs(iterations - iterations_old);
    if(iteration_diff > 10)
    {
        std::cout << "Error: iteration count differences greater 10 (diff: " 
            << iteration_diff << ")" << std::endl;
    }

    /*decltype(auto) first = stencil_code::get_first(grid_old);
  
    for (auto i = 0u; i < n; ++i) 
    {
        for (auto j = 0u; j < n; ++j) 
        {
            if(std::abs(first.at({i,j}) - grid2.get(i,j)) < 0.001) continue;

            std::cout << "Error at " << i << "|" << j 
                << ", old: " << first.at({i,j}) << "    new: " << grid2.get(i,j)
                << std::endl;

            throw std::runtime_error("incorrect result");
        }
    }
    */

    return EXIT_SUCCESS;
}
