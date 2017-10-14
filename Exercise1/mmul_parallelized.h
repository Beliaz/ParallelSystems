#ifndef MMUL_PARALLELIZED_H
#define MMUL_PARALLELIZED_H

#include "mmul_optimized_2.h"

namespace mmul_parallelized
{
	static constexpr auto threshold = 32;

	using matrix_storage = mmul_optimized_2::matrix_storage;
	using matrix = mmul_optimized_2::matrix;

	inline decltype(auto) data(matrix& m) { return mmul_optimized_2::data(m); }
	inline decltype(auto) data(const matrix& m) { return mmul_optimized_2::data(m); }
	inline auto dim(const matrix& m) { return mmul_optimized_2::dim(m); }

	inline matrix make_matrix(const size_t n)
	{
		return mmul_optimized_2::make_matrix(n);
	}

	inline matrix id(const unsigned n)
	{
		return mmul_optimized_2::id(n);
	}

	inline matrix transpose(const matrix& m)
	{
		using namespace mmul_optimized_2;

		const auto n = static_cast<int>(dim(m));

		auto o = make_matrix(n);

		#pragma omp parallel
		for (auto i = 0; i < n; ++i)
		{
			#pragma omp for nowait
			for (auto j = 0; j < n; ++j)
			{
				data(o)[j*n + i] = data(m)[i*n + j];
			}
		}

		return o;
	}

	// computes the product of two matrices
	inline matrix operator*(const matrix& a, const matrix& b)
	{
		const auto n = static_cast<int>(dim(a));

		if (n < threshold)
			return mmul_optimized_2::operator*(a, b);

		auto c = make_matrix(n);
		const auto b_trans = transpose(b);

		#pragma omp parallel
		for (auto i = 0; i < n; ++i)
		{
			const auto row_a = i * n;

			#pragma omp for nowait
			for (auto j = 0; j < n; ++j)
			{
				const auto row_b = j * n;

				auto sum = 0.0;

				for (auto k = 0; k < n; ++k)
				{
					sum +=  data(a)[row_a  + k] *
							data(b_trans)[row_b + k];
				}

				data(c)[row_a + j] = sum;
			}
		}

		return c;
	}
}

#endif
