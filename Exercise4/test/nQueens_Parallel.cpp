#include <malloc.h>
#include <iostream>
#include <bits/stl_algo.h>
#include <stdlib.h>
#include <libltdl/lt_system.h>
#include <ctgmath>
#include <vector>

#define TEST 1
#if TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif



bool checkValidity(int problemSize, int *permutation) {
    for (int i = 0; i < problemSize; i++) {
        for (int j = 0; j<problemSize; j++) {
            if (i == j)
                continue;
            if (abs(i-j) == abs(permutation[i] - permutation[j]))
                return false;
        }
    }
    return true;
}

bool checkValidity(int problemSize, std::vector<int> permutation) {
    int *array = &permutation[0];
    return checkValidity(problemSize, array);
}

int findPermutations(int problemSize) {
    std::vector<int> base((unsigned long) problemSize);
    for (int i = 0; i < problemSize; i++) {
        base[i] = i+1;
    }
    int *foundPermutations = new int[problemSize];

#pragma omp parallel for
    for (int i = 0; i<problemSize;i++) {
        int found = 0;
        auto tmp = base;
        std::rotate(tmp.begin(), tmp.begin() + i, tmp.begin() + i + 1);
        do {
            if (checkValidity(problemSize, tmp))
                found++;
        } while (std::next_permutation(tmp.begin() + 1, tmp.end()));
        foundPermutations[i] = found;
    }

    //Count together all the found possibilities
    int totalFound = 0;
    for (int i = 0; i<problemSize;i++) {
        totalFound+=foundPermutations[i];
    }
    return totalFound;
}

#if TEST
TEST_CASE( "standard chess" ) {
    REQUIRE( findPermutations(8) == 92 );
}

TEST_CASE( "3x3 gives zero" ) {
    REQUIRE( findPermutations(3) == 0 );
}

TEST_CASE( "field of 10") {
    REQUIRE( findPermutations(10) == 724 );
}

TEST_CASE( "field of 9") {
    REQUIRE( findPermutations(9) == 352 );
}

TEST_CASE( "field of 5") {
    REQUIRE( findPermutations(5) == 10 );
}

#endif

#if !TEST
int main(int argc, char** argv) {
    //Do some testing whether the given input is correct.
    if (argc != 2) {
        printf("Please give exactly one problem size");
        return EXIT_FAILURE;

    }
    if (atoi(argv[1]) > 17) {
        printf("Please enter a smaller problem size as yours might take too long");
        return EXIT_FAILURE;
    }


    int problemSize = atoi(argv[1]);
    int foundPermutations = findPermutations(problemSize);

    printf("I totally found %d solutions", foundPermutations);
}
#endif