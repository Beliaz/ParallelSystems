#include <malloc.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

int factorial(int num) {
    int sum = 1;
    for (int i = 1;i<=num;i++) {
        sum*=i;
    }
    return sum;
}

//Saves the permutations into our array
void createPermutations(int problemSize, int *permutations) {
    int *tmp = new int[problemSize];
    for (int i = 0; i < problemSize; i++) {
        tmp[i] = i+1;
    }

    int permutation = 0;
    do {
        for (int i = 0; i < problemSize; i++) {
            //std::cout << tmp[i] << ' ';
            permutations[permutation*problemSize + i] = tmp[i];
        }
        //std::cout << '\n';
        permutation++;
    } while ( std::next_permutation(tmp,tmp+problemSize) );
}

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

int checkPermutations(int problemSize, int permutationCount, int *permutations) {
    int count = 0;
    for (int i = 0; i < permutationCount; i++) {
        if (checkValidity(problemSize, permutations+(problemSize*i)))
            count++;
    }
    return count;
}

int main(int argc, char** argv) {
    //Do some testing whether the given input is correct.
    if (argc != 2) {
        printf("Please give exactly one problem size");
        return EXIT_FAILURE;

    }
    if (atoi(argv[1]) > 11) {
        printf("Please enter a smaller problem size as yours might take too long");
        return EXIT_FAILURE;
    }


    int problemSize = atoi(argv[1]);
    int foundPermutations = 0;
    int permutationCount = factorial(problemSize);
    int *permutations = new int[problemSize * permutationCount] ;

    createPermutations(problemSize, permutations);
    foundPermutations = checkPermutations(problemSize, permutationCount, permutations);

    printf("found %d possibilities\n", foundPermutations);

}