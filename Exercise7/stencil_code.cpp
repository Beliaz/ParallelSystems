#include <cstdio>
#include <iostream>
#include <cmath>
#include <ctime>
#include "baseDef.h"
#include "1d.h"
#include "2d.h"
#include "3d.h"

bool inputTester(int argc, char **argv) {
    //Dimension + Size always needed + 2*dimension for borders
    //Resulting in 4, 6 or 8 arguments needed; don't forget to add the name of the program as well
    for (int i = 1; i < argc; i++)
        if (std::isnan(atoi(argv[i])))
            return false;
    return (argc == 5 && atoi(argv[1]) == 1 || argc == 7 && atoi(argv[1]) == 2 || argc == 9 && atoi(argv[1]) == 3);
}

unsigned long calculate(SIZETYPE size, TYPE *borders, int dimension) {
    unsigned long duration = 0;
    switch (dimension) {
        default:
        case 1:
            duration = calculate1D(size, borders);
            break;
        case 2:
            duration = calculate2D(size, borders);
            break;
        case 3:
            duration = calculate3D(size, borders);
            break;
    }
    return duration;
}

int main(int argc, char **argv) {
    if (!inputTester(argc, argv)) {
        std::cout << "Arguments don't match, please try again\n" << "Usage is ./programName dimension problemSize 2-6 border values\n"
                  << "1D starts with the left value, 2D with top, then bottom, then left, then right\n"
                  << "3D takes top, bottom, left, right, back, front" << std::endl;
        return EXIT_FAILURE;
    }

    std::srand(std::time(0));
    int dimension = atoi(argv[1]);
    SIZETYPE size = atoi(argv[2]);
    if (dimension > 3 || dimension < 1 || size <= 0)
        return EXIT_FAILURE;

    //Array used to write the border cases
    TYPE *borders = new TYPE[size];
    for (int i = 0; i < dimension * 2; i++)
        borders[i] = atoi(argv[i+3]);

    std::cout << "Took " << calculate(size, borders, dimension) << "ms to finish the job" << std::endl;
    return EXIT_SUCCESS;
}