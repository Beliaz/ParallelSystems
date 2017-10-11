#include "benchmark/benchmark_api.h"
#include "../mmul.h"
#include <iostream>
#include "../build/googlebenchmark-src/src/statistics.h"

static void matrix_multiplication(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		const auto n = state.range_x();

		auto a = id(n);
		a[0][0] = 42;

		const auto b = id(n);

		const auto c = a * b;

		state.PauseTiming();

		state.SetComplexityN(n);

		const auto correct = c == a;

		state.ResumeTiming();

		if (!correct) throw std::exception();
	}
}

BENCHMARK(matrix_multiplication)
->RangeMultiplier(2)
->Range(8, 8 << 6)
->ReportAggregatesOnly()
->Repetitions(5)
->Complexity();

/*->ComputeStatistics("standrad deviation", benchmark::StatisticsStdDev)
->ComputeStatistics("median", benchmark::StatisticsMedian)
->ComputeStatistics("mean", benchmark::StatisticsMean);*/

BENCHMARK_MAIN();