#include <iostream>
#ifndef TYPE
#include "baseDef.h"
#endif
#ifndef time_ms
#include "time_ms.h"
#endif

#ifndef PARALLELSYSTEMSTEAM_3D_H
#define PARALLELSYSTEMSTEAM_3D_H

#endif //PARALLELSYSTEMSTEAM_3D_H

void print3D(TYPE *array, SIZETYPE size) {
    for (int i = 0; i < size + 2; i++) {
        for (SIZETYPE j = 0; j <= size + 1; j++) {
            for (SIZETYPE k = 0; k <= size + 1; k++) {
                //array[i*size*size + j*size + k] = i*size*size + j*size + k;
                TYPE value = array[i * (size + 2) * (size + 2) + j * (size + 2) + k];
                std::cout << value << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

//Create and write the random values to start with
void autoFill3D(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 1; i < size + 1; i++)
        for (SIZETYPE j = 1; j < size + 1; j++)
            for (SIZETYPE k = 1; k < size + 1; k++)
                array[i * (size + 2) * (size + 2) + j * (size + 2) + k] = 0;
}

//Write Borders so we do not need any border cases.
//Needs some crazy position calculations as there is only a one dimensional array used.
//However, the first layer of size (size + 2)² is the top one, so the value in the left top corner got index 0
void create3DBorders(TYPE *array, SIZETYPE size, TYPE *borders) {
    for (SIZETYPE i = 0; i < (size + 2) * (size + 2); i++) {
        array[i] = borders[0];
    }
    for (SIZETYPE i = 0; i < (size + 2) * (size + 2); i++) {
        array[(size+2)*(size + 2)*(size+1)+i] = borders[1];
    }
    for (SIZETYPE i = 0; i < (size + 2) * (size + 2); i++) {
        long position = i*(size + 2);
        array[position] = borders[2];
        array[position + size + 1] = borders[3];
    }
    int counter = 0;
    for (SIZETYPE i = 1; i <= (size + 2) * (size + 2); i++) {
        array[(size+2) * (size+2) * counter + i%(size + 2)] = borders[4];
        array[(size+2) * (size+2) * (counter + 1) - (i%(size+2)+1)] = borders[5];

        if (i != 0 && i%(size + 2) == 0)
            counter++;
    }
}

//Does the job and returns the delta Epsilon accumulated over all comparisons.
TYPE iteration3D(TYPE *source, TYPE *target, SIZETYPE size) {
    TYPE dEpsilon = 0;
#pragma omp parallel for reduction (+ : dEpsilon)
    for (SIZETYPE i = 1; i < size + 1; i++) {
        for (SIZETYPE j = 1; j < size + 1; j++) {
            for (SIZETYPE k = 1; k < size + 1; k++) {
                TYPE current = source[i * (size + 2) * (size + 2) + j * (size + 2) + k];
                //Using seven points as it is necessary to take the local value into account as well
                TYPE newValue = (TYPE) ((
                        source[i * (size + 2) * (size + 2) + j * (size + 2) + k - 1] +
                        source[i * (size + 2) * (size + 2) + j * (size + 2) + k + 1] +
                        source[i * (size + 2) * (size + 2) + j * (size + 2) + k] +
                        source[i * (size + 2) * (size + 2) + (j-1) * (size + 2) + k] +
                        source[i * (size + 2) * (size + 2) + (j+1) * (size + 2) + k] +
                        source[(i - 1) * (size + 2) * (size + 2) + (j-1) * (size + 2) + k] +
                        source[(i + 1) * (size + 2) * (size + 2) + (j-1) * (size + 2) + k]) / 7);

                target[i * (size + 2) * (size + 2) + j * (size + 2) + k] = newValue;
                dEpsilon += current - newValue > 0 ? current - newValue : newValue - current;
            }
        }
    }
    return dEpsilon;
}

//Function to control and start the computation. Does measure the time as well and returns it
unsigned long calculate3D(SIZETYPE size, TYPE *borders) {
    auto *array1 = initArray(size, 3);
    auto *array2 = initArray(size, 3);
    autoFill3D(array1, size);
    create3DBorders(array1, size, borders);
    create3DBorders(array2, size, borders);

    //Start to measure time
    unsigned long startTime = time_ms();
    unsigned long finishTime;
    unsigned iterations = 0;

    //Always do two iterations, as the arrays have to switch every time. This way, it is not needed to keep track which was the last
    while (true) {

        iteration3D(array1, array2, size);
        TYPE dEpsilon = iteration3D(array2, array1, size);

        iterations += 2;

        if (dEpsilon < epsilonStop) {
            //Finish measuring time
            finishTime = time_ms();
            if (DOPRINT)
                print1D(array1, size);
            break;
        }
    }

    printf("iterations: %i\n", iterations);

    return finishTime - startTime;
}