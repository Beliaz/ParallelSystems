#ifndef JACOBI_ITERATION_H
#define JACOBI_ITERATION_H

#include "benchmark/benchmark.h"
#include "../stencil.h"
#include "../grid_helper.h"

static void jacobi_iteration_1d_impl(benchmark::State& state)
{
    constexpr auto dim = 1;

    const auto n = static_cast<size_t>(state.range(0));

    state.SetComplexityN(static_cast<int>(n));

    auto grid = create_grid<cell_t, dim>({ n }, { 1, 1 }, { 0, 0 });

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(stencil::iteration<dim>
            ::template execute<stencil::jacobi_iteration, 0>(grid));
    }
}

static void jacobi_iteration_2d_impl(benchmark::State& state)
{
    constexpr auto dim = 2;

    const auto n = static_cast<size_t>(state.range(0));

    state.SetComplexityN(static_cast<int>(n));

    auto grid = create_grid<cell_t, dim>({ n, n }, { 1, 1, 1, 1 }, { 0, 0 });

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(stencil::iteration<dim>
            ::template execute<stencil::jacobi_iteration, 0>(grid));
    }
}

static void jacobi_iteration_3d_impl(benchmark::State& state)
{
    constexpr auto dim = 3;

    const auto n = static_cast<size_t>(state.range(0));

    state.SetComplexityN(static_cast<int>(n));

    auto grid = create_grid<cell_t, dim>({ n, n, n }, { 1, 1, 1, 1, 1, 1 }, { 0, 0 });

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(stencil::iteration<dim>
            ::template execute<stencil::jacobi_iteration, 0>(grid));
    }
}

#define JACOBI_ITERATION_1_D_BENCHMARK(f) \
    BENCHMARK(f) \
    ->RangeMultiplier(2) \
    ->Range(1024, 2 << 19) \
    ->ReportAggregatesOnly() \
    ->Repetitions(5) \
    ->Complexity(); \

#define JACOBI_ITERATION_2_D_BENCHMARK(f) \
    BENCHMARK(f) \
    ->RangeMultiplier(2) \
    ->Range(16, 2 << 9) \
    ->ReportAggregatesOnly() \
    ->Repetitions(5) \
    ->Complexity();

#define JACOBI_ITERATION_3_D_BENCHMARK(f) \
    BENCHMARK(f) \
    ->RangeMultiplier(2) \
    ->Range(16, 2 << 7) \
    ->ReportAggregatesOnly() \
    ->Repetitions(5) \
    ->Complexity();

#endif // JACOBI_ITERATION_H

