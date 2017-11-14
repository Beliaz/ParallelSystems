#ifndef MERGE_SORT_BENCHMARK_H
#define MERGE_SORT_BENCHMARK_H

#include <iostream>
#include "benchmark/benchmark.h"
#include "../merge_sort.h"
#include <algorithm>

#define CUSTOM_BENCHMARK(f) BENCHMARK(f) \
->RangeMultiplier(2) \
->Range(32768, 2 << 20) \
->ReportAggregatesOnly() \
->Complexity() \
->Unit(benchmark::kMillisecond) \
//->Repetitions(5) \

static void merge_sort(benchmark::State& state)
{
    const auto n = state.range(0);

    state.SetComplexityN(n);

    auto arr = init(n);
    /*
#if defined(STD_STABLE)
     while (state.KeepRunning())
     {
        std::sort(arr.begin(),arr.end());
        if (!is_sorted(arr))
        {
            state.SkipWithError("result incorrect");
        }
    }
#elif defined(STD_QUICK)
    while (state.KeepRunning())
     {
        std::stable_sort(arr.begin(),arr.end());
        if (!is_sorted(arr))
        {
            state.SkipWithError("result incorrect");
        }
    }
#else
    while (state.KeepRunning())
    {
        auto result = sort(arr);
        if ((!is_sorted(result)) || result.size()!=arr.size())
        {
            state.SkipWithError("result incorrect");
        }
    }
#endif
     */
}

#endif // MERGE_SORT_BENCHMARK_H
