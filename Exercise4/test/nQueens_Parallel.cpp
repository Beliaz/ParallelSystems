#include <malloc.h>
#include <iostream>
#include <stdlib.h>
#include <ctgmath>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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
