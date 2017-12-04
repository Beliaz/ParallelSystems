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

//Create and write the random values to start with
void autoFill(TYPE *array, SIZETYPE size) {
#pragma omp parallel for
    for (SIZETYPE i = 0; i < size + 2; i++) {
        array[i] = 0;
    }
}

//Does the job and returns the delta Epsilon accumulated over all comparisons.
TYPE iteration(TYPE *source, TYPE *target, SIZETYPE size) {
    TYPE dEpsilon = 0;
#pragma omp parallel for reduction (+ : dEpsilon)
    for (SIZETYPE i = 1; i < size + 1; i++) {
        TYPE current = source[i];
        //Using three points as it is necessary to take the local value into account as well
        TYPE newValue = (TYPE) ((source[i-1] + source[i] + source[i+1]) / 3);
        target[i] = newValue;
        dEpsilon += current - newValue > 0 ? current - newValue : newValue - current;
    }
    return dEpsilon;
}

//Function to control and start the computation. Does measure the time as well and returns it
unsigned long calculate1D(SIZETYPE size, TYPE *borders) {
    int buffer = 5;
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    size = ceil((double) size / world_size) + buffer * 2; //buffer times 2 because there are two sides which need information
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    auto *array1 = initArray(size-2, 1);
    auto *array2 = initArray(size-2, 1);
    //Creates some random values to start with
    //autoFill(array1, size-2);
    //autoFill(array2, size-2);
    //Write Borders so we do not need any border cases.
    if (world_rank == 0)
        array1[0] = array2[0] = borders[0];
    else if (world_rank == world_size - 1)
        array1[size-1] = array2[size-1] = borders[1];

    //Start to measure time
    unsigned long startTime = time_ms();
    unsigned long finishTime;
    unsigned iterations = 0;

    //Always do two iterations, as the arrays have to switch every time. This way, it is not needed to keep track which was the last
    TYPE dEpsilon;

    print1D(array1, size-2);
    while (true) {

        iteration(array1, array2, size-2);
        dEpsilon = iteration(array2, array1, size-2);
        //print1D(array2, size-1);


        iterations += 2;
        print1D(array1, size-2);

        if (dEpsilon < epsilonStop) {
            //Finish measuring time
            finishTime = time_ms();
            if (DOPRINT)
                print1D(array1, size);
            break;
        }
    }

    const unsigned long elapsed = finishTime - startTime;

    printf("%lu ms, %i iter", elapsed, iterations);

    if (elapsed > 0)
    {
        printf(", %.3e cells/s",
            iterations * size / (double)elapsed * 1000);
    }

    printf("\n");

    MPI_Finalize();
    return finishTime - startTime;
}