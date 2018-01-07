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

    blocks = static_cast<int>(sqrt(num_procs));

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
    const auto iterations = s.execute(primary, secondary);

    MPI_Finalize();

    if (my_rank != 0) return EXIT_SUCCESS;

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            clock::now() - start).count();
    std::cout << elapsed << ";" << iterations << ";" << std::scientific << std::setprecision(3)
                  << iterations * size / static_cast<double>(elapsed) * 1000 << std::endl;

    return EXIT_SUCCESS;
}
