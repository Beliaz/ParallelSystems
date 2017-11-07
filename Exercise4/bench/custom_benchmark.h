#ifndef CUSTOM_BENCHMARK_H
#define CUSTOM_BENCHMARK_H


#include "benchmark/benchmark.h"
#include "../merge_sort.h"

#define CUSTOM_BENCHMARK(f) BENCHMARK(f) \
->RangeMultiplier(2) \
->Range(64, 8 << 10) \
->ReportAggregatesOnly() \
->Repetitions(5) \
->Complexity() \

static void merge_sort(benchmark::State& state)
{
    const auto n = state.range(0);

    state.SetComplexityN(n);

    const auto arr = init(n);

     while (state.KeepRunning())
     {
        if (!is_sorted(sort(arr)))
        {
            state.SkipWithError("result incorrect");
        }
    }
}

#endif // CUSTOM_BENCHMARK_H
