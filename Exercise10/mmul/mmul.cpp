#include "stdlib.h"
#include <mpi.h>
#include "gsl/gsl"
#include <random>
#include <iostream>
#include <chrono>

#include "matrix.h"
#include "summa_seq.h"

using value_t = double;
using distribution_t = std::uniform_real_distribution<value_t>;
using matrix_t = matrix<value_t>;
using technique_t = summa_sequential;

template<typename DistributionType>
class matrix_factory
{
public:
    template<typename T>
    matrix<T> create_random(size_t n) const
    {
        auto m = matrix<T>(n);

        auto gen = std::bind(_distribution, engine_);

        generate(m.begin(), m.end(), gen);

        return m;
    }

    template<typename T>
    static matrix<T> create_identity(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        for(size_type i = 0; i < n; ++i)
        {   
            m(i, i) = static_cast<T>(1);
        }

        return m;
    }

    template<typename T>
    static matrix<T> create_range(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        auto current = 0;
        for (size_type i = 0; i < n; ++i)
        {
            for (size_type j = 0; j < n; ++j)
            {
                m(i, j) = static_cast<T>(++current);
            }
        }

        return m;
    }

    template<typename T>
    static matrix<T> create_index_range(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        for (size_type i = 0; i < n; ++i)
        {
            for (size_type j = 0; j < n; ++j)
            {
                m(i, j) = gsl::narrow<T>(i * 10 + j);
            }
        }

        return m;
    }

private:
    std::default_random_engine engine_;
    DistributionType _distribution;
};

int main(int argc, char* argv[])
{
    using namespace std::chrono;

    using clock = high_resolution_clock;
    using factory_t = matrix_factory<distribution_t>;

    if(argc != 2)
    {
        std::cout << "usage: mmul <n> [<approach>]" << std::endl;
        return 1;
    }

    MPI_Init(&argc, &argv);

    const auto start = clock::now();

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    //const auto n = atol(argv[1]);
    const auto n = 4;

    const auto factory = factory_t();

    const auto a = my_rank == 0
        ? factory_t::create_index_range<value_t>(n)
        : matrix_t(n);

    const auto b = my_rank == 0
       // ? factory_t::create_index_range<value_t>(n)
        ? factory_t::create_identity<value_t>(n)
        : matrix_t(n);

    const auto c = multiply(a, b, technique_t());
        
    MPI_Finalize();

    if(my_rank != 0) return EXIT_SUCCESS;

    const auto diff = duration_cast<milliseconds>(clock::now() - start);

    std::cout << diff.count() << " ms" << std::endl;

    std::cin.get();

    Ensures(c == a);
    
    return EXIT_SUCCESS;
}
