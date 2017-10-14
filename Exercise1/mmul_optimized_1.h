#ifndef MMUL_OPTIMIZED_1_H
#define MMUL_OPTIMIZED_1_H

#include <vector>
#include <memory>

namespace mmul_optimized_1
{
	using matrix_storage = std::unique_ptr<double[]>;
	using matrix = std::pair<matrix_storage, size_t>;

	inline decltype(auto) data(matrix& m) { return std::get<0>(m); }
	inline decltype(auto) data(const matrix& m) { return std::get<0>(m); }
	inline auto dim(const matrix& m) { return std::get<1>(m); }

	inline matrix make_matrix(const size_t n)
	{
		return {std::make_unique<double[]>(n*n), n };
	}

	// initializes a square identity matrix of size n x n
	inline matrix id(const unsigned n) 
	{
		auto m = make_matrix(n);

		for (unsigned i = 0; i < n; i++)
		{
			for (unsigned j = 0; j < n; j++)
			{
				data(m)[i * n + j] = 0;
			}

			data(m)[i * n + i] = 1;
		}

		return m;
	}

	// computes the product of two matrices
	inline matrix operator*(const matrix& a, const matrix& b)
	{
		const auto n = dim(a);

		auto c = make_matrix(n);

		for (unsigned i = 0; i < n; ++i)
		{
			const auto row_a = i * n;

			for (unsigned j = 0; j < n; ++j)
			{
				auto sum = 0.0;

				for (unsigned k = 0; k < n; ++k)
				{
					sum +=  data(a)[row_a + k] *
							data(b)[k * n + j];
				}

				data(c)[row_a + j] = sum;
			}
		}

		return c;
	}
}

#endif
