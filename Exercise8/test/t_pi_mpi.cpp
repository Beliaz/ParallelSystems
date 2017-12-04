#include "../pi/pi.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "../pi/pi_mpi.h"

int main(int argc, char** argv)
{
    constexpr auto samples = 10'000'000;

    int my_rank = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    try
    {
        if (my_rank == 0)
        {
            const auto pi = pi_mpi::master(samples);

            const auto rounded_pi = static_cast<int>(floor(pi * 10)) / 10;
            const auto rounded_real_pi = static_cast<int>(floor(M_PI * 10)) / 10;

            if (rounded_pi != rounded_real_pi)
                throw std::runtime_error("estimate incorrect: " + 
                    std::to_string(rounded_pi));
        }
        else
        {
            pi_mpi::slave();
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;

        MPI_Finalize();
        return EXIT_FAILURE;
    }

    MPI_Finalize();
    return MPI_SUCCESS;
}
