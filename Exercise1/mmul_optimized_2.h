#ifndef MMUL_OPTIMIZED_2_H
#define MMUL_OPTIMIZED_2_H

#include "mmul_optimized_1.h"

namespace mmul_optimized_2
{
	using matrix_storage = mmul_optimized_1::matrix_storage;
	using matrix = mmul_optimized_1::matrix;

	inline decltype(auto) data(matrix& m) { return mmul_optimized_1::data(m); }
	inline decltype(auto) data(const matrix& m) { return mmul_optimized_1::data(m); }
	inline auto dim(const matrix& m) { return mmul_optimized_1::dim(m); }

	inline matrix make_matrix(const size_t n)
	{
		return mmul_optimized_1::make_matrix(n);
	}

	// initializes a square identity matrix of size n x n
	inline matrix id(const unsigned n)
	{
		return mmul_optimized_1::id(n);
	}

	inline mmul_optimized_1::matrix transpose(const mmul_optimized_1::matrix& m)
	{
		using namespace mmul_optimized_1;

		const auto n = dim(m);

		auto o = make_matrix(n);

		for (unsigned i = 0; i < n; ++i)
		{
			for (auto j = i; j < n; ++j)
			{
				data(o)[i*n + j] = data(m)[j*n + i];
				data(o)[j*n + i] = data(m)[i*n + j];
			}
		}

		return o;
	}

	// computes the product of two matrices
	inline mmul_optimized_1::matrix operator*(const mmul_optimized_1::matrix& a, const mmul_optimized_1::matrix& b)
	{
		using namespace mmul_optimized_1;

		const auto n = dim(a);

		auto c = make_matrix(n);
		const auto b_trans = transpose(b);

		for (unsigned i = 0; i < n; ++i)
		{
			const auto row_a = i * n;

			for (unsigned j = 0; j < n; ++j)
			{
				const auto row_b = j * n;

				auto sum = 0.0;

				for (unsigned k = 0; k < n; ++k)
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
