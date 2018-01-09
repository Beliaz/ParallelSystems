#include "mmul.h"
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    auto num_procs = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    auto rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const auto n = 128;

    const auto a = rank == 0
        ? factory_t::create_index_range(n)
        : matrix_t(n);

    const auto b = rank == 0
        ? factory_t::create_identity(n)
        : matrix_t(n);

    const auto c = multiply(a, b, summa_distributed());

    MPI_Finalize();

    if (rank != 0) return EXIT_SUCCESS;

    Expects(c == a);

    return c == a 
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
