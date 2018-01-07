#include "stdlib.h"
#include <mpi.h>
#include <vector>
#include "gsl/gsl"
#include <random>
#include <iostream>
#include <chrono>

struct naive_sequential { };
struct summa_sequential { };

template<typename T>
class matrix
{
public:
    using size_type = size_t;
    using value_type = T;

    explicit matrix(
        const size_type n, 
        const value_type default_value = value_type())
        : row_size_(n), data_(n * n, default_value)
    {
        
    }

    matrix& operator = (const matrix& other)
    {
        if (&other == this) return *this;

        Expects(row_size_ == other.row_size_);

        data_ = other.data_;

        return *this;
    }

    T& operator ()(const size_type row, const size_type column)
    {
        return data_[row * row_size_ + column];
    }

    T operator ()(const size_type row, const size_type column) const
    {
        return data_[row * row_size_ + column];
    }

    size_type n() const { return row_size_; }

    value_type* begin() { return &data_.front(); }
    value_type* end() { return &data_.back(); }

    const value_type* begin() const { return &data_.front(); }
    const value_type* end() const { return &data_.back(); }

private:
    const size_t row_size_;
    std::vector<T> data_;
};

using value_t = double;
using distribution_t = std::uniform_real_distribution<value_t>;
using matrix_t = matrix<value_t>;
using technique_t = naive_sequential;

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
template<typename T, typename TagType>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, TagType tag)
{
    return multiply(a, b, tag);
}

template<typename T>
bool operator == (const matrix<T>& a, const matrix<T>& b)
{
    return std::equal(a.begin(), a.end(), b.begin());
}

template<typename DistributionType>
class matrix_factory
{
public:
    template<typename T>
    matrix<T> create_random(size_t n) const
    {
        auto m = matrix<T>(n);

        auto gen = std::bind(_distribution, engine_);

        generate(m.begin(), m.end(), gen);

        return m;
    }

    template<typename T>
    static matrix<T> create_identity(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        for(size_type i = 0; i < n; ++i)
        {   
            m(i, i) = static_cast<T>(1);
        }

        return m;
    }

    template<typename T>
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

private:
    std::default_random_engine engine_;
    DistributionType _distribution;
};

int main(int argc, char* argv[])
{
    using namespace std::chrono;

        using clock = high_resolution_clock;
    using factory_t = matrix_factory<distribution_t>;

    if(argc != 2)
    {
        std::cout << "usage: mmul <n> [<approach>]" << std::endl;
        return 1;
    }

    MPI_Init(&argc, &argv);

    const auto start = clock::now();

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    //const auto n = atol(argv[1]);
    const auto n = 4;

    const auto factory = factory_t();

    const auto a = my_rank == 0
        ? factory_t::create_range<value_t>(n)
        : matrix_t(n);

    const auto b = my_rank == 0
        ? factory.create_identity<value_t>(n)
        : matrix_t(n);

    const auto c = multiply(a, b, technique_t());
        
    MPI_Finalize();

    if(my_rank != 0) return EXIT_SUCCESS;

    const auto diff = duration_cast<milliseconds>(clock::now() - start);

    std::cout << diff.count() << " ms" << std::endl;

    Ensures(c == a);

    std::cin.get();
    
    return EXIT_SUCCESS;
}
