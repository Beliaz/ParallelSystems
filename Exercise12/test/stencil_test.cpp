//
// Created by ivan on 11/12/17.
//

#include <cmath>
#include <string>
#include <iostream>
#include <chrono>

#define NTEST

#include "../mpi_stencil.h"
#include "sequential/jacobi.h"
#include "sequential/iteration.h"
#include "sequential/stencil_code.h"
#include "sequential/print.h"

#include <mpi.h>
#include <thread>

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
        MPI_Finalize();
        throw std::runtime_error("processor count not supported");
    }

    communicator = MPI_COMM_WORLD;

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

    constexpr ::bounds_t<2> bounds = { 0.5, -0.5, 1, 0 };
    const stencil_code::grid_extents_t<2> extents = { n - 2, n - 2 };

    auto grid_old = create_buffered_grid(extents, bounds, default_value);

    decltype(auto) first = stencil_code::get_first(grid_old);
    decltype(auto) second = stencil_code::get_second(grid_old);

    for (auto i = 0; i < iterations; i += 2)
    {
        using iteration_type = stencil_code::iteration<stencil_code::get_dim<decltype(grid_old)>()>;

        iteration_type::template execute<stencil_code::jacobi>(first, second);
        iteration_type::template execute<stencil_code::jacobi>(second, first);
    }    

    if(my_rank == 0)
    {
        std::cout << "iterations: " << iterations << std::endl << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // evaluate
    decltype(auto) expected = stencil_code::get_first(grid_old);
    const auto& actual = grid1;

    auto error = false;

    for (auto y = actual.top_y(); y < actual.bottom_y(); ++y) 
    {
        for (auto x = actual.left_x(); x < actual.right_x(); ++x) 
        {
            //skip corners
            if (y == 0 && x == 0) continue;
            if (y == 0 && x == n - 1) continue;
            if (y == n - 1 && x == 0) continue;
            if (y == n - 1 && x == n - 1) continue;

            const auto e = expected.at({ x, y });
            const auto a = actual.get(y, x);

            const auto diff = std::abs(e - a);

            if(diff < 0.01) continue;

            std::cout << std::fixed << std::setprecision(3);
            std::cout << "(" << std::setw(2) << x << ", " << std::setw(2) << y
                << ") e: " << std::setw(6) << e
                << " a: " << std::setw(6) << a
                << " d: " << std::setw(5) << diff
                << std::endl;

            error = true;
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    if (my_rank != 0)
    {
        return error
            ? EXIT_FAILURE
            : EXIT_SUCCESS;
    }

    std::cout << "Expected values: " << std::endl;

    std::cout << std::endl;

    print(expected);

    std::cout << std::endl;

    std::cout << "Actual values: " << std::endl;

    std::cout << std::endl;

    actual.print();

    std::cout << std::endl;

    return EXIT_SUCCESS;
}
