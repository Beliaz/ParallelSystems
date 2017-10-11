#include "../mmul.h"
#include <cstdlib>

int main(int, char**) 
{
	const auto n = 10;

    auto a = id(n);
    a[0][0] = 42;

	const auto b = id(n);

    // compute the product
    const auto c = a * b;

    // check that the result is correct
    return c == a ? EXIT_SUCCESS : EXIT_FAILURE;
}