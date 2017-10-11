#include "mmul.h"
#include <cstdlib>

int main(int argc, char** argv) {

    if(argc!=2) return EXIT_FAILURE;
    unsigned n = atoi(argv[1]);
    if(n==0) return EXIT_FAILURE;

    // create two matrices
    auto a = id(n);
    a[0][0] = 42;
    auto b = id(n);

    // compute the product
    auto c = a * b;

    // check that the result is correct
    return (c == a) ? EXIT_SUCCESS : EXIT_FAILURE;
}