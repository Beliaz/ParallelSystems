#include "pi_benchmark.h"

static void pi_seq(benchmark::State& state)
{
    const auto n = state.range(0);

    state.SetComplexityN(n);

    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(pi::calculate<pi::method::seq>(n));
    }
}

CUSTOM_BENCHMARK(pi_seq);

BENCHMARK_MAIN();