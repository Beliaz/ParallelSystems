#ifndef PI_BENCHMARK_H
#define PI_BENCHMARK_H

#include "benchmark/benchmark.h"
#include "../src/pi/pi.h"

#define CUSTOM_BENCHMARK(f) BENCHMARK(f) \
->RangeMultiplier(2) \
->Range(1024, 2 << 20) \
->ReportAggregatesOnly() \
->Repetitions(5) \
->Complexity() \

#endif // PI_BENCHMARK_H
