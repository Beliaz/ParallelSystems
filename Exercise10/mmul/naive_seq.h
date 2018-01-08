#ifndef NAIVE_SEQ_H
#define NAIVE_SEQ_H

#include "gsl/gsl_assert"
#include "matrix.h"

struct naive_sequential { };

template<typename T>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, naive_sequential)
{
    Expects(a.n() == b.n());

    const auto n = a.n();

    auto c = matrix<T>(a.n(), 0);

    using size_type = typename matrix<T>::size_type;

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

#endif // NAIVE_SEQ_H
