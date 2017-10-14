#include "../mmul_parallelized.h"
#include <cstdlib>
#include <iostream>

using namespace mmul_parallelized;

template<unsigned N>
void test_identity_matrix()
{
	const auto m = id(N);

	for (auto i = 0u; i < N; i++)
	{
		for (auto j = 0u; j < N; j++)
		{
			auto mij = data(m)[i*N + j];

			if (i == j && mij == 1) continue;
			if (i != j && mij == 0) continue;

			throw "test_identity_matrix failed";
		}
	}
}

template<unsigned N>
void test_matrix_multiplication()
{
	auto a = id(N);
	data(a)[0] = 42;

	const auto b = id(N);

	// compute the product
	const auto c = a * b;

	for (auto i = 0u; i < N; i++)
	{
		for (auto j = 0u; j < N; j++)
		{
			if(data(c)[i * N + j] == data(a)[i * N + j])
				continue;

			throw "test_matrix_multiplication failed";
		}
	}
}

int main(int, char**)
{
	try
	{
		test_identity_matrix<10>();
		test_matrix_multiplication<10>();

		return EXIT_SUCCESS;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << "\n";
		return EXIT_FAILURE;
	}
}