#include <malloc.h>
#include <iostream>

int* initArray (int problemSize, int i, int j) {
    auto* array = new int[problemSize];
    for (int i = 0; i < problemSize; i++)
        array[i] = -1;
    array[0] = i + 1;
    array[1] = j + 1;
    return array;
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

bool includes(int number, int *array, int problemSize) {
    for (int i = 0; i < problemSize; i++)
        if (number == array[i])
            return true;
    return false;
}

int recursion(int *array, int remainingSize, int problemSize) {
    int found = 0;
    for (int i = 0; i < problemSize; i++) {
        if (includes(i + 1, array, problemSize - remainingSize))
            continue;
        array[problemSize - remainingSize] = i + 1;
        if (remainingSize == 1) {
            if (checkValidity(problemSize, array))
                found++;
            array[problemSize - remainingSize] = -1;
        }
        else
            found += recursion(array, remainingSize - 1, problemSize);
    }
    return found;
}

int findIterations(int problemSize) {
    int sum = 0;
#pragma omp parallel for shared(problemSize) reduction(+:sum)
    for (int i = 0; i < problemSize; i++) {
#pragma omp parallel for
        for (int j = 0; j < problemSize; j++) {
            if (i == j)
                continue;
            auto array = initArray(problemSize, i, j);
            sum += recursion(array, problemSize - 2, problemSize);
        }
    }
    return sum;
}

int main(void) {
    int size = 10;
    printf("Found %d iterations\n",findIterations(size));
    return 0;
}