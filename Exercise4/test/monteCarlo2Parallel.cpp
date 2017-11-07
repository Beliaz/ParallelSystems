#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <omp.h>

int throwLoop(int throwCount) {
    double x, y;

    int totalHits = 0;
#pragma omp parallel reduction(+:totalHits)
    {
        int hitCount = 0;

#pragma omp for schedule(static)
        for (int i = 0; i < throwCount; i++) {

#ifdef WIN32
            x = (double)(double)rand() / RAND_MAX;
            y = (double)(double)rand() / RAND_MAX;
#else
            x = (double) (double) rand_r(&myseed) / RAND_MAX;
            y = (double) (double) rand_r(&myseed) / RAND_MAX;
#endif
            if ((x * x + y * y <= 1))
                hitCount++;
        }
        printf("found %d hits\n", hitCount);
        totalHits+=hitCount;
    }
    return totalHits;
}

double getResult(int throwCount, int hitCount) {
    return (double) hitCount / throwCount * 4;
}

int main(void) {
    int throwCount = 10000000;
    int hitCount = throwLoop(throwCount);
    printf("Pi seems to be %f\n", getResult(throwCount, hitCount));
}
