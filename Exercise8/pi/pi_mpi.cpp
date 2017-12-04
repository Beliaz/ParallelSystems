#include <cstdlib>
#include "pi.h"
#include <iostream>
#include <string>
#include "../chrono_timer.h"
#include "mpi.h"

int main(int argc, char** argv)
{
    int my_rank = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // master
    if(my_rank == 0)
    {
        if (argc != 2)
        {
            std::cout << "usage: pi_par_2 <number_of_samples>" << std::endl;
            return EXIT_FAILURE;
        }

        const auto samples = std::stoull(argv[1]);

        int num_procs = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

        auto samples_per_proc = samples / num_procs;

        for (auto i = 1; i < num_procs; ++i)
        {
            auto status = MPI_SUCCESS;
            if((status = MPI_Send(&samples_per_proc, 1, MPI_INT, i, 0, MPI_COMM_WORLD)) 
                != MPI_SUCCESS)
            {
                std::cerr << "error while sending sample count to slaves" << std::endl;
                return status;
            }
        }

        const auto local_samples = samples_per_proc + samples % num_procs;

        {
            ChronoTimer timer("PI_MPI");

            auto pi = pi::calculate<pi::method::par_2>(local_samples);

            float slave_pi = 0;
            MPI_Status status;
            for (auto i = 1; i < num_procs; ++i)
            {
                MPI_Recv(&slave_pi, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

                pi += slave_pi;
            }

            pi /= num_procs;

            //std::cout << std::endl;
            std::cout << floor(pi * 10000) / 10000 << " - ";
        };

        //std::cout << std::endl;
    }
    else
    {
        MPI_Status status;
        auto samples = 0;

        MPI_Recv(&samples, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        //std::cout << "slave(" << my_rank << "): " << samples << " samples" << std::endl;

        const auto pi = pi::calculate<pi::method::par_2>(samples);

        //std::cout << "slave(" << my_rank << "): finished" << std::endl;

        MPI_Send(&pi, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return MPI_SUCCESS;
}
