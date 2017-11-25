#include <cstdio>
#include <iostream>
#include <cmath>
#include <ctime>

#define TYPE float
#define SIZETYPE long

void print1D(TYPE *array, SIZETYPE size) {
    for (SIZETYPE i = 0; i <= size+1; i++) {
        TYPE value = array[i];
        std::cout << value << " | ";
    }
    std::cout << std::endl;
}

void print2D(TYPE *array, SIZETYPE size) {
    for (int j = 0; j < size + 2; j++) {
        for (SIZETYPE i = 0; i <= size + 1; i++) {
            TYPE value = array[j*(size+2) + i];
            std::cout << value << " | ";
        }
        std::cout << std::endl;
    }
}

void autoFill(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 1; i < size + 1; i++) {
        array[i] = (TYPE) (std::rand() % 600) + 173;
    }
}

void autoFill2D(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 1; i < size + 1; i++) {
        for (SIZETYPE j = 1; j < size + 1; j++)
            array[i*(size+2)+j] = (TYPE) (std::rand() % 600) + 173;
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

TYPE* initArray(SIZETYPE size, short dimension) {
    TYPE *array = nullptr;
    switch (dimension) {
        default:
        case 1:
            array = new TYPE[size + 2];
            break;
        case 2:
            array = new TYPE[(size+2)*(size+2)];
            break;
        case 3:
            array = new TYPE[(size+2)*(size+2)*(size+2)];
            break;
    }
    return array;
}

void calculate1D(SIZETYPE size, TYPE *borders) {
    auto *array1 = initArray(size, 1);
    auto *array2 = initArray(size, 1);
    autoFill(array1, size);
    //Write Borders so we do not need any border cases.
    array1[0] = array2[0] = borders[0];
    array1[size+1] = array2[size+1] = borders[1];

    while(true){
        iteration(array1, array2, size);

        TYPE dEpsilon = iteration(array2, array1, size);
        if (dEpsilon < 1) {
            print1D(array1, size);
            break;
        }
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

int main() {
    std::cout << std::endl;
    std::srand(std::time(0));
    short dimension = 2;
    SIZETYPE size = 200;
    if (dimension > 3 || dimension < 1)
        return EXIT_FAILURE;
    if (size <= 0)
        return EXIT_FAILURE;


    TYPE *borders;
    switch (dimension) {
        default:
        case 1:
            borders = new TYPE[2];
            borders[0] = 0;
            borders[1] = 65535;
            calculate1D(size, borders);
        case 2:
            borders = new TYPE[4];
            borders[0] = 1000;
            borders[1] = 5000;
            borders[2] = 4000;
            borders[3] = 8000;
            calculate2D(size, borders);
    }
}