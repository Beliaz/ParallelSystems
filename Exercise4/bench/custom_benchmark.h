#ifndef CUSTOM_BENCHMARK_H
#define CUSTOM_BENCHMARK_H

#include "benchmark/benchmark.h"
#include "../merge_sort.h"

#define CUSTOM_BENCHMARK(f) BENCHMARK(f) \
->RangeMultiplier(2) \
->Range(16, 8 << 5) \
->ReportAggregatesOnly() \
->Repetitions(5) \
->Complexity() \

static void multiplication(benchmark::State& state)
{
  const auto n = state.range(0);

  state.SetComplexityN(n);

  // create two matrices
  auto a = init(n, [](unsigned i, unsigned j) { return (i == 0) && (j == 0) ? 42.0 : 0.0; });
  auto b = init(n, [](unsigned i, unsigned j) { return (i == j) ? 1.0 : 0.0; });

  while (state.KeepRunning())
  {
	if (a * b != a)
	{
	  state.SkipWithError("result incorrect");
	}
  }
}

#endif // CUSTOM_BENCHMARK_H
