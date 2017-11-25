#include <iostream>
#ifndef TYPE
#include "baseDef.h"
#endif

#ifndef PARALLELSYSTEMSTEAM_2D_H
#define PARALLELSYSTEMSTEAM_2D_H

#endif //PARALLELSYSTEMSTEAM_2D_H

void print2D(TYPE *array, SIZETYPE size) {
    for (int j = 0; j < size + 2; j++) {
        for (SIZETYPE i = 0; i <= size + 1; i++) {
            TYPE value = array[j*(size+2) + i];
            std::cout << value << " | ";
        }
        std::cout << std::endl;
    }
}

void autoFill2D(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 1; i < size + 1; i++) {
        for (SIZETYPE j = 1; j < size + 1; j++)
            array[i*(size+2)+j] = (TYPE) (std::rand() % 600) + 173;
    }
}

void create2DBorders(TYPE *array, SIZETYPE size, TYPE *borders) {
    for (SIZETYPE i = 0; i < size + 2; i++) {
        array[i] = borders[0];
    }
    for (SIZETYPE i = 0; i < size + 2; i++) {
        array[(size+2)*(size+1)+i] = borders[1];
    }
    for (SIZETYPE i = 0; i < size + 2; i++) {
        array[(size+2)*i] = borders[2];
        array[(size+2)*i+(size+1)] = borders[3];
    }
}

TYPE iteration2D(TYPE *source, TYPE *target, SIZETYPE size) {
    TYPE dEpsilon = 0;
#pragma omp parallel for reduction (+ : dEpsilon)
    for (SIZETYPE i = 1; i < size + 1; i++) {
        for (SIZETYPE j = 1; j < size + 1; j++) {
            TYPE current = source[i*(size+2)+j];
            TYPE newValue = (TYPE) ((source[i*(size+2)+j-1] + source[i*(size+2)+j] + source[i*(size+2)+j+1] + source[(i-1)*(size+2)+j] + source[(i+1)*(size+2)+j]) / 5);
            target[i*(size+2)+j] = newValue;
            dEpsilon += current - newValue > 0 ? current - newValue : newValue - current;

        }
    }
    return dEpsilon;
}

void calculate2D(SIZETYPE size, TYPE *borders) {
    auto *array1 = initArray(size, 2);
    auto *array2 = initArray(size, 2);
    create2DBorders(array1, size, borders);
    create2DBorders(array2, size, borders);
    autoFill2D(array1, size);

    while(true){
        iteration2D(array1, array2, size);

        TYPE dEpsilon = iteration2D(array2, array1, size);
        if (dEpsilon < 1) {
            print2D(array1, size);
            break;
        }
    }
}