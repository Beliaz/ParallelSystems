#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "time_ms.h"

typedef uint32_t uint;
uint full, *qs, nn;
uint count = 0;
uint totalCount = 0;

void solve(uint d, uint c, uint l, uint r)
{
#pragma omp critical
    totalCount++;
    uint b, a, *s;
    if (!d) {
#pragma omp critical
        count++;
        return;
    }

    a = (c | (l <<= 1) | (r >>= 1)) & full;
    if (a != full)
        for (*(s = qs + --d) = 0, b = 1; b <= full; (*s)++, b <<= 1)
            if (!(b & a)) {
#pragma omp task
                solve(d, b|c, b|l, b|r);
            }
}

int main(int n, char **argv)
{
    if (n <= 1 || (nn = atoi(argv[1])) <= 0) nn = 8;

    qs = calloc(nn, sizeof(int));
    full = (1U << nn) - 1;

    unsigned long start_time = time_ms();
#pragma omp parallel
    {
#pragma omp single
        solve(nn, 0, 0, 0);
    }

    printf("\nSolutions: %d\n", count);
    printf("\nCalculation steps: %d\n", totalCount);
    // print run time
    printf("Time: %9llu ms\n", time_ms() - start_time);
    return 0;
}