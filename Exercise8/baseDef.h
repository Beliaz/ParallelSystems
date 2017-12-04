#ifndef PARALLELSYSTEMSTEAM_BASEDEF_H
#define PARALLELSYSTEMSTEAM_BASEDEF_H

#endif //PARALLELSYSTEMSTEAM_BASEDEF_H

#define TYPE float
#define SIZETYPE long
#define epsilonStop 1


#ifndef DOPRINT
#define DOPRINT false
#endif

#include <mpi.h>

void autoFillBase(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 0; i < size + 2; i++) {
        array[i] = 0;
    }
}
//Creates an array based on the dimension an size
TYPE* initArray(SIZETYPE size, short dimension) {
    TYPE *array = nullptr;
    switch (dimension) {
        default:
        case 1:
            array = new TYPE[size + 2];
            autoFillBase(array, size);
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