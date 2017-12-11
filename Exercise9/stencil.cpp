//
// Created by ivan on 04/12/17.
//

#define GSL_THROW_ON_CONTRACT_VIOLATION

#include <iostream>
#include <chrono>
#include <iomanip>
#include <mpi.h>
#include <cmath>
#include <array>

#include "gsl/gsl"
#include "grid.h"
#include "mpi_stencil.h"

constexpr auto epsilon = 20;

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

int execute(const stencil &s, grid& primary, grid& secondary)
{
    auto iterations = 0;

    while (true)
    {
        s.iteration(primary, secondary);
        s.send_recv_border(primary);

        const auto d_epsilon = s.iteration(secondary, primary);
        s.send_recv_border(primary);

        iterations += 2;

        double sum_epsilon;
        MPI_Allreduce(&d_epsilon, &sum_epsilon, 1, MPI_DOUBLE, MPI_SUM, communicator);

        if (sum_epsilon < epsilon)
            break;
    }

    return iterations;
}

#endif

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    auto num_procs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    const auto blocks = static_cast<int>(sqrt(num_procs));

    if (blocks * blocks != num_procs)
    {
        if (my_rank >= blocks * blocks )
        {
            MPI_Comm_split(MPI_COMM_WORLD, 1, my_rank, &communicator);
            MPI_Finalize();
            return EXIT_SUCCESS;
        }

        MPI_Comm_split(MPI_COMM_WORLD, 0, my_rank, &communicator);
    } 
    else
    {
        communicator = MPI_COMM_WORLD;
    }

    MPI_Comm_size(communicator, &num_procs);
    MPI_Comm_rank(communicator, &my_rank);

    const std::array<const double, 4> borders
    {
        1.0, 0.5, 0, -0.5
    };

    grid primary(my_rank, blocks, borders);
    grid secondary(my_rank, blocks, borders);

    const stencil s(num_procs, my_rank, primary);

    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();

    ///////////////////////////////////////////////////////////
    // Actual loop
    const auto iterations = execute(s, primary, secondary);

    MPI_Finalize();

    if (my_rank != 0) return EXIT_SUCCESS;

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            clock::now() - start).count();

    std::cout << elapsed << " ms, " << iterations << " iter";

    if(elapsed > 0)
    {
        std::cout << ", " << std::scientific << std::setprecision(3)
                  << iterations * size / static_cast<double>(elapsed) * 1000
                  << " cells/s";
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}
