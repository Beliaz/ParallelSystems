#include "stdlib.h"
#include <mpi.h>
#include "gsl/gsl"
#include <random>
#include <iostream>
#include <chrono>

#include "mmul.h"

int main(int argc, char* argv[])
{
    using namespace std::chrono;

    using clock = high_resolution_clock;

    if(argc != 2)
    {
        std::cout << "usage: mmul <n> [<approach>]" << std::endl;
        return 1;
    }

    MPI_Init(&argc, &argv);

    const auto start = clock::now();

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    const auto n = atol(argv[1]);

    const auto a = my_rank == 0
        ? factory_t::create_index_range(n)
        : matrix_t(n);

    const auto b = my_rank == 0
        ? factory_t::create_identity(n)
        : matrix_t(n);

    const auto c = multiply(a, b, summa_distributed{ 4, true });
        
    MPI_Finalize();

    if(my_rank != 0) return EXIT_SUCCESS;

    const auto diff = duration_cast<milliseconds>(clock::now() - start);

    std::cout << diff.count() << " ms" << std::endl;

    std::cin.get();

    if(!(c == a))
    {
        std::cout << "incorrect result" << std::endl;
    }
    
    return EXIT_SUCCESS;
}
