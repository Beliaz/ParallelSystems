#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <algorithm>

#include "gsl/gsl"

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

    matrix& operator += (const matrix other)
    {
        std::transform(data_.begin(), data_.end(),
            other.data_.begin(),
            data_.begin(),
            std::plus<value_type>());

        return *this;
    }

    bool operator == (const matrix other) const
    {
        return std::equal(begin(), end(), other.begin());
    }
    
    size_type n() const { return row_size_; }
    size_type size() const { return row_size_ * row_size_; }

    value_type* begin() { return &data_.front(); }
    value_type* end() { return &data_.back(); }

    const value_type* begin() const { return &data_.front(); }
    const value_type* end() const { return &data_.back(); }

private:
    const size_t row_size_;
    std::vector<T> data_;
};

template<typename T, typename TagType>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, TagType tag) = delete;

#endif // MATRIX_H
