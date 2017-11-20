#ifndef AVL_BENCHMARK_H
#define AVL_BENCHMARK_H

#include "benchmark/benchmark.h"
#include "../avl.h"
#include <chrono>
#include <random>

#define CUSTOM_BENCHMARK(f) BENCHMARK(f) \
->RangeMultiplier(2) \
->Range(1024, 2 << 19) \
->ReportAggregatesOnly() \
->Repetitions(5) \
->Complexity() \

static void avl_insert(benchmark::State& state)
{
    const auto n = state.range(0);

    state.SetComplexityN(n);

    std::default_random_engine random_engine;
    std::uniform_int_distribution<unsigned int> range(0, n / 8);

    random_engine.seed(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    
    std::vector<unsigned int> list(n, 0);
    generate(list.begin(), list.end(), [&]() { return range(random_engine); });

    for(const auto _ : state)
    {
        avlTree avl;

        avl.insert(list);

        if (avl.check_order()) continue;
            
        state.SkipWithError("ordering invariant not preserved");
    }
}

CUSTOM_BENCHMARK(avl_insert);

BENCHMARK_MAIN();

#endif // AVL_BENCHMARK_H
