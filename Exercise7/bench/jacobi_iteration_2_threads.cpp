#include <omp.h>

#include "jacobi_iteration_benchmark.h"

constexpr auto num_threads = 2;

static void jacobi_iteration_1d(benchmark::State& state)
{
    omp_set_num_threads(num_threads);
    jacobi_iteration_1d_impl(state);
}

static void jacobi_iteration_2d(benchmark::State& state)
{
    omp_set_num_threads(num_threads);
    jacobi_iteration_2d_impl(state);
}

static void jacobi_iteration_3d(benchmark::State& state)
{
    omp_set_num_threads(num_threads);
    jacobi_iteration_3d_impl(state);
}

JACOBI_ITERATION_1_D_BENCHMARK(jacobi_iteration_1d)

JACOBI_ITERATION_2_D_BENCHMARK(jacobi_iteration_2d)

JACOBI_ITERATION_3_D_BENCHMARK(jacobi_iteration_3d)

BENCHMARK_MAIN()