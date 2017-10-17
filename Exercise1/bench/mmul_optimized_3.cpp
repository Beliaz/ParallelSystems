#include <vector>
#include "custom_benchmark.h"

using Matrix = std::vector<std::vector<double>>;

// initializes a square identity matrix of size n x n
Matrix id(unsigned n) {
    Matrix res;
    res.resize(n);
    for(unsigned i=0; i<n; i++) {
        res[i].resize(n);
        for(unsigned j=0; j<n; j++) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }
    return res;
}

// initializes a square zero matrix of size n x n
Matrix zero(unsigned n) {
    Matrix res;
    res.resize(n);
    for(unsigned i=0; i<n; i++) {
        res[i].resize(n);
        for(unsigned j=0; j<n; j++) {
            res[i][j] = 0;
        }
    }
    return res;
}

// computes the product of two matrices
Matrix operator*(const Matrix& a, const Matrix& b) {
    unsigned n = static_cast<unsigned>(a.size());
    Matrix c = zero(n);
    for(unsigned i=0; i<n; ++i) {
        for(unsigned k=0; k<n; ++k) {
            for(unsigned j=0; j<n; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

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
