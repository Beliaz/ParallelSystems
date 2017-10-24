#define CONTIGUOUS_WITH_MULTIPLICATION

#include "../mmul.h"
#include <cstdlib>
#include "../chrono_timer.h"

#if !defined(NESTED_VECTOR) && !defined(CONTIGUOUS_WITH_MULTIPLICATION) && !defined(CONTIGUOUS_WITH_INDIRECTION)
#define NESTED_VECTOR
#endif


int main(int argc, char** argv) {

    if (argc != 2) return EXIT_FAILURE;
    unsigned n = atoi(argv[1]);
    if (n == 0) return EXIT_FAILURE;

    // create two matrices
    auto a = init(n, [](unsigned i, unsigned j) { return (i == 0) && (j == 0) ? 42.0 : 0.0; });
    auto b = init(n, [](unsigned i, unsigned j) { return (i == j) ? 1.0 : 0.0; });

    Matrix c;
    // compute the product
    {
        ChronoTimer t("Multiplication");
        c = a * b;
    }

    // check that the result is correct
    auto ret = (c == a) ? EXIT_SUCCESS : EXIT_FAILURE;
    std::cout << "Ret: " << ret << std::endl;
    return ret;
}