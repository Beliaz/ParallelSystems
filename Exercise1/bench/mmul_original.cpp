#include "../mmul.h"
#include "custom_benchmark.h"

static void identity(benchmark::State& state)
{
	const auto n = state.range(0);

	state.SetComplexityN(n);

	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(id(n));
	}
}

CUSTOM_BENCHMARK(identity);

static void multiplication(benchmark::State& state)
{
	const auto n = state.range(0);

	state.SetComplexityN(n);

	const auto a = [&]()
	{
		auto m = id(n);
		m[0][0] = 42;

		return m;
	}();

	const auto b = id(n);

	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(a * b);
	}
}

CUSTOM_BENCHMARK(multiplication);

BENCHMARK_MAIN();