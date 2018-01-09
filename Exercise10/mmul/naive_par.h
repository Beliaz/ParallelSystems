#ifndef NAIVE_PAR_H
#define NAIVE_PAR_H

#include "gsl/gsl_assert"
#include "matrix.h"

constexpr auto par_threshold = 128;

struct naive_parallel { };

template<typename T>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, naive_parallel)
{
    Expects(a.n() == b.n());

    const auto n = a.n();

    auto c = matrix<T>(a.n(), 0);

    using size_type = typename matrix<T>::size_type;

    #pragma omp parallel for if(n > par_threshold)
    for (size_type i = 0; i < n; ++i)
    {
        for (size_type k = 0; k < n; ++k)
        {
            for (size_type j = 0; j < n; ++j)
            {
                c(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    return c;
}

#endif // NAIVE_PAR_H
