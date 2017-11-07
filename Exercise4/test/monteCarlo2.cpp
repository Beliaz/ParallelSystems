#include <stdio.h>
#include <cstdlib>
#include <ctime>

int throwLoop(int throwCount) {
    double x, y;

        int hitCount = 0;
        for (int i = 0; i < throwCount; i++) {
            x = (double) (double) rand() / RAND_MAX;
            y = (double) (double) rand() / RAND_MAX;

            if ((x * x + y * y <= 1))
                hitCount++;
        }
        printf("found %d hits\n", hitCount);
    return hitCount;
}

double getResult(int throwCount, int hitCount) {
    return (double) hitCount / throwCount * 4;
}

int main(void) {
    srand (time(NULL));
    int throwCount = 1000000;
    int hitCount = throwLoop(throwCount);
    printf("Pi seems to be %f\n", getResult(throwCount, hitCount));
}