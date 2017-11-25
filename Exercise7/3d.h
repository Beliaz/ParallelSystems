#include <iostream>
#ifndef TYPE
#include "baseDef.h"
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

void create3DBorders(TYPE *array, SIZETYPE size, TYPE *borders) {
    for (SIZETYPE i = 0; i < (size + 2) * (size + 2); i++) {
        array[i] = borders[0];
    }
    for (SIZETYPE i = 0; i < (size + 2) * (size + 2); i++) {
        array[(size+2)*(size + 2)*(size+1)+i] = borders[1];
    }
    int counter = 0;
    for (SIZETYPE i = 1; i <= (size + 2) * (size + 2); i++) {
        //short version for position i*(size + 2) …
        long position = (size + 2) * (size + 2) * (i % (size + 2)) + (size+2) * counter;
        array[position] = borders[2];
        array[position + size + 1] = borders[3];
        if (i != 0 && i%(size + 2) == 0)
            counter++;
    }
    counter = 0;
    for (SIZETYPE i = 1; i <= (size + 2) * (size + 2); i++) {
        long position = (size + 2) *(size + 2) * counter + i%(size + 2);
        array[position] = borders[4];
        array[(size+2) * (size + 2) * (counter + 1) - (i % 4 + 1)] = borders[5];

        if (i != 0 && i%(size + 2) == 0)
            counter++;
    }
}

/*void calculate2D(SIZETYPE size, TYPE *borders) {
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
}*/

void calculate3D(SIZETYPE size, TYPE *borders) {
    auto *array1 = initArray(size, 3);
    auto *array2 = initArray(size, 3);
    //print3D(array1, size);
    create3DBorders(array1, size, borders);
    print3D(array1, size);
}