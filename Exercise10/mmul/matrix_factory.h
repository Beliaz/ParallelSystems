#ifndef MATRIX_FACTORY_H
#define MATRIX_FACTORY_H

#include "matrix.h"
#include <random>

template<typename T>
class matrix_factory
{
public:
    matrix<T> create_random(size_t n) const
    {
        auto m = matrix<T>(n);

        auto gen = std::bind(distribution_, engine_);

        generate(m.begin(), m.end(), gen);

        return m;
    }

    static matrix<T> create_identity(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        for (size_type i = 0; i < n; ++i)
        {
            m(i, i) = static_cast<T>(1);
        }

        return m;
    }

    static matrix<T> create_range(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        auto current = 0;
        for (size_type i = 0; i < n; ++i)
        {
            for (size_type j = 0; j < n; ++j)
            {
                m(i, j) = static_cast<T>(++current);
            }
        }

        return m;
    }

    static matrix<T> create_index_range(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        for (size_type i = 0; i < n; ++i)
        {
            for (size_type j = 0; j < n; ++j)
            {
                m(i, j) = gsl::narrow<T>(i * 10 + j);
            }
        }

        return m;
    }

private:
    static std::default_random_engine engine_;
    static std::uniform_real_distribution<T> distribution_;
};
#endif // MATRIX_FACTORY_H
