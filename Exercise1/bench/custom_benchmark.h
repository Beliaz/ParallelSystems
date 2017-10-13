#ifndef CUSTOM_BENCHMARK_H
#define CUSTOM_BENCHMARK_H

#include "benchmark/benchmark.h"

#define CUSTOM_BENCHMARK(f) BENCHMARK(f) \
->RangeMultiplier(2) \
->Range(8, 8 << 5) \
->ReportAggregatesOnly() \
->Repetitions(5) \
->Complexity() \

#endif // CUSTOM_BENCHMARK_H
