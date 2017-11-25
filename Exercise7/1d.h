#include <iostream>
#ifndef TYPE
#include "baseDef.h"
#endif
#ifndef time_ms
#include "time_ms.h"

#endif

#ifndef PARALLELSYSTEMSTEAM_1D_H
#define PARALLELSYSTEMSTEAM_1D_H

#endif //PARALLELSYSTEMSTEAM_1D_H

void print1D(TYPE *array, SIZETYPE size) {
    for (SIZETYPE i = 0; i <= size+1; i++) {
        TYPE value = array[i];
        std::cout << value << " | ";
    }
    std::cout << std::endl;
}

void autoFill(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 1; i < size + 1; i++) {
        array[i] = (TYPE) (std::rand() % 600) + 173;
    }
}

TYPE iteration(TYPE *source, TYPE *target, SIZETYPE size) {
    TYPE dEpsilon = 0;
#pragma omp parallel for reduction (+ : dEpsilon)
    for (SIZETYPE i = 1; i < size + 1; i++) {
        TYPE current = source[i];
        TYPE newValue = (TYPE) ((source[i-1] + source[i] + source[i+1]) / 3);
        target[i] = newValue;
        dEpsilon += current - newValue > 0 ? current - newValue : newValue - current;
    }
    return dEpsilon;
}

unsigned long calculate1D(SIZETYPE size, TYPE *borders) {
    auto *array1 = initArray(size, 1);
    auto *array2 = initArray(size, 1);
    autoFill(array1, size);
    //Write Borders so we do not need any border cases.
    array1[0] = array2[0] = borders[0];
    array1[size+1] = array2[size+1] = borders[1];

    unsigned long startTime = time_ms();
    unsigned long finishTime;
    while(true){
        iteration(array1, array2, size);

        TYPE dEpsilon = iteration(array2, array1, size);
        if (dEpsilon < epsilonStop) {
            finishTime = time_ms();
            print1D(array1, size);
            break;
        }
    }
    return finishTime - startTime;
}