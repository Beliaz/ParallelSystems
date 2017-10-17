#ifndef MMUL_OPTIMIZED_2_H
#define MMUL_OPTIMIZED_2_H

#include "mmul_optimized_1.h"

namespace mmul_optimized_4
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

	inline matrix transpose(const mmul_optimized_1::matrix& m)
	{
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

	inline matrix zero(const size_t n)
	{
		auto m = make_matrix(n);

		for (auto i = 0u; i < n * n; ++i)
			data(m)[i] = 0.0;

		return m;
	}

	// computes the product of two matrices
	inline matrix operator*(const mmul_optimized_1::matrix& a, const mmul_optimized_1::matrix& b)
	{
		const auto n = dim(a);

		auto c = zero(n);

		for (unsigned i = 0; i < n; ++i)	
			for (unsigned k = 0; k < n; ++k)	
				for (unsigned j = 0; j < n; ++j)
					data(c)[i * n + j] += 
						data(a)[i * n + k] *
						data(b)[k * n + j];

		return c;
	}
}

#endif
