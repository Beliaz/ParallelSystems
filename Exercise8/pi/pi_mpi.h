#ifndef PI_MPI_H
#define PI_MPI_H

#include "pi.h"
#include "mpi.h"
#include <iostream>

namespace pi_mpi
{
    pi::value_t master(auto samples)
    {
        int num_procs = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

        auto samples_per_proc = samples / num_procs;

        for (auto i = 1; i < num_procs; ++i)
        {
            auto status = MPI_SUCCESS;
            if ((status = MPI_Send(&samples_per_proc, 1, MPI_INT, i, 0, MPI_COMM_WORLD)) 
                != MPI_SUCCESS)
            {
                throw std::runtime_error("error while sending sample count to slaves");
            }
        }

        const auto local_samples = samples_per_proc + samples % num_procs;

        auto pi = pi::calculate<pi::method::par_2>(local_samples);

        auto slave_pi = 0.0f;
        MPI_Status status;

        for (auto i = 1; i < num_procs; ++i)
        {
            if(MPI_Recv(&slave_pi, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status)
                    != MPI_SUCCESS)
            {
                throw std::runtime_error("error receving pi estimations from slaves");
            }

            pi += slave_pi;
        }

        return pi / num_procs;
    }

    void slave()
    {
        MPI_Status status;
        auto samples = 0;

        if(MPI_Recv(&samples, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status) != MPI_SUCCESS)
            throw std::runtime_error("error receiving sample count");

        //std::cout << "slave(" << my_rank << "): " << samples << " samples" << std::endl;

        const auto pi = pi::calculate<pi::method::par_2>(samples);

        //std::cout << "slave(" << my_rank << "): finished" << std::endl;

        if(MPI_Send(&pi, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD) != MPI_SUCCESS)
            throw std::runtime_error("error sending pi estimation");
    }
}

#endif