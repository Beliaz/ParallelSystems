#include "../mmul.h"
#include <cstdlib>

template<unsigned N>
void test_identity_matrix()
{
	const auto m = id(N);

	for(auto i = 0u; i < N; i++)
	{
		for (auto j = 0u; j < N; j++)
		{
			auto mij = m[i][j];

			if (i == j && mij == 1) continue;
			if (i != j && mij == 0) continue;

			throw;
		}
	}
}

template<unsigned N>
void test_matrix_multiplication()
{
	auto a = id(N);
	a[0][0] = 42;

	const auto b = id(N);

	// compute the product
	const auto c = a * b;

	// check that the result is correct
	if(c != a) throw;
}

int main(int, char**) 
{
	try
	{
		test_identity_matrix<10>();
		test_matrix_multiplication<10>();

		return EXIT_SUCCESS;
	}
	catch(...)
	{
		return EXIT_FAILURE;
	}
}