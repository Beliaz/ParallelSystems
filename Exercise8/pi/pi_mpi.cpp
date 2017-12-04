#include <cstdlib>
#include "pi.h"
#include <iostream>
#include <string>
#include "../chrono_timer.h"
#include "pi_mpi.h"

int main(int argc, char** argv)
{
    int my_rank = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    try
    {
        if (my_rank == 0)
        {
            if (argc != 2)
            {
                std::cout << "usage: pi_par_2 <number_of_samples>" << std::endl;
                return EXIT_FAILURE;
            }

            const auto samples = std::stoull(argv[1]);

            {
                ChronoTimer timer("PI_MPI");

                const auto pi = pi_mpi::master(samples);

                //std::cout << std::endl;
                std::cout << floor(pi * 10000) / 10000 << " - ";
            };

            //std::cout << std::endl;
        }
        else
        {
            pi_mpi::slave();
        }
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;

        MPI_Finalize();
        return EXIT_FAILURE;
    }
    
    MPI_Finalize();
    return MPI_SUCCESS;
}
