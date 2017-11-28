#ifndef JACOBI_ITERATION_BENCHMARK_H
#define JACOBI_ITERATION_BENCHMARK_H

#include "benchmark/benchmark.h"
#include "../stencil.h"
#include "../grid_helper.h"
#include "../iteration.h"
#include "../jacobi.h"

constexpr auto num_iterations = 100;

static void jacobi_iteration_1d_impl(benchmark::State& state)
{
    using namespace stencil;

    constexpr auto dim = 1;

    const auto n = static_cast<size_t>(state.range(0));

    state.SetComplexityN(static_cast<int>(n));

    while (state.KeepRunning())
    {
        state.PauseTiming();

        auto grid = create_buffered_grid(
            grid_extents_t<1>{ n }, 
            { 1, 1 }, 
            default_value);

        decltype(auto) first = stencil::get_first(grid);
        decltype(auto) second = stencil::get_second(grid);

        state.ResumeTiming();

        for (auto i = 0u; i < num_iterations; i++)
        {
            benchmark::DoNotOptimize(iteration<dim>
                ::execute<jacobi>(first, second));

            benchmark::DoNotOptimize(iteration<dim>
                ::execute<jacobi>(second, first));
        }
    }

    state.SetItemsProcessed(n * state.iterations() * num_iterations * 2);
}

static void jacobi_iteration_2d_impl(benchmark::State& state)
{
    using namespace stencil;

    constexpr auto dim = 2;

    const auto n = static_cast<size_t>(state.range(0));

    state.SetComplexityN(static_cast<int>(n));

    while (state.KeepRunning())
    {
        state.PauseTiming();

        auto grid = create_buffered_grid(
            grid_extents_t<2>{ n, n }, 
            { 1, 1, 1, 1 }, 
            default_value);

        decltype(auto) first = stencil::get_first(grid);
        decltype(auto) second = stencil::get_second(grid);

        state.ResumeTiming();

        for (auto i = 0u; i < num_iterations; i++)
        {
            benchmark::DoNotOptimize(iteration<dim>
                ::execute<jacobi>(first, second));

            benchmark::DoNotOptimize(iteration<dim>
                ::execute<jacobi>(second, first));
        }
    }

    state.SetItemsProcessed(n * n * state.iterations() * num_iterations * 2);
}

static void jacobi_iteration_3d_impl(benchmark::State& state)
{
    using namespace stencil;

    constexpr auto dim = 3;

    const auto n = static_cast<size_t>(state.range(0));

    state.SetComplexityN(static_cast<int>(n));

    while (state.KeepRunning())
    {
        state.PauseTiming();

        auto grid = create_buffered_grid(
            grid_extents_t<3>{ n, n, n }, 
            { 1, 1, 1, 1, 1, 1 },
            default_value);

        decltype(auto) first = stencil::get_first(grid);
        decltype(auto) second = stencil::get_second(grid);

        state.ResumeTiming();

        for (auto i = 0u; i < num_iterations; i++)
        {
            benchmark::DoNotOptimize(iteration<dim>
                ::execute<jacobi>(first, second));

            benchmark::DoNotOptimize(iteration<dim>
                ::execute<jacobi>(second, first));
        }
    }

    state.SetItemsProcessed(n * n * n * state.iterations() * num_iterations * 2);
}

#define JACOBI_ITERATION_1_D_BENCHMARK(f) \
    BENCHMARK(f) \
    ->RangeMultiplier(2) \
    ->Range(4096, 2 << 15) \
    ->ReportAggregatesOnly() \
    ->Repetitions(5) \
    ->Complexity(); \

#define JACOBI_ITERATION_2_D_BENCHMARK(f) \
    BENCHMARK(f) \
    ->RangeMultiplier(2) \
    ->Range(32, 2 << 9) \
    ->ReportAggregatesOnly() \
    ->Repetitions(5) \
    ->Complexity();

#define JACOBI_ITERATION_3_D_BENCHMARK(f) \
    BENCHMARK(f) \
    ->RangeMultiplier(2) \
    ->Range(32, 2 << 7) \
    ->ReportAggregatesOnly() \
    ->Repetitions(5) \
    ->Complexity();

#endif // JACOBI_ITERATION_BENCHMARK_H

