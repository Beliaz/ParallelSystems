#include "stdlib.h"
#include <mpi.h>
#include <vector>
#include "gsl/gsl"
#include <random>
#include <iostream>
#include <chrono>
#include <iomanip>

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

    matrix& operator += (const matrix other)
    {
        std::transform(data_.begin(), data_.end(), 
            other.data_.begin(), 
            data_.begin(), 
            std::plus<value_type>());

        return *this;
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
using technique_t = summa_sequential;

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

size_t get_num_blocks_per_dim(const size_t n, const size_t num_procs)
{
    Expects(n > 0);
    Expects(num_procs > 0);

    if (num_procs == 1) return 1;

    const auto procs_per_dim = gsl::narrow<size_t>(
        std::floor(std::sqrt(num_procs)));

    const auto num_procs_used = procs_per_dim * procs_per_dim;

    if (num_procs_used != num_procs)
    {
        std::cout << "warning only using "
            << num_procs_used << " out of "
            << num_procs << " processors " << std::endl;

    }

    if(n % procs_per_dim != 0)
    {
        std::cout << "error: n (" << n 
            << ") not divisible by processor count (" 
            << procs_per_dim << ")" << std::endl;

        throw;
    }

    return procs_per_dim;
}

template<typename T>
matrix<T> compute_outer_product(
    const matrix<T>& a, 
    const typename matrix<T>::size_type column_index,
    const matrix<T>& b, 
    const typename matrix<T>::size_type row_index)
{
    Expects(a.n() == b.n());

    matrix<T> c(a.n());

    for(size_t i = 0; i < a.n(); ++i)
    {
        for (size_t j = 0; j < b.n(); ++j)
        {
            c(i, j) = a(i, column_index) * b(row_index, j);
        }
    }

    return c;
}

template<typename T>
void print_blocks(std::vector<std::vector<std::array<matrix<T>, 3>>> blocks)
{
    const auto block_size = blocks[0][0][0].n();
    const auto num_blocks = blocks.size();
    const auto n = block_size * num_blocks;

    for (size_t i = 0; i < n; ++i)
    {
        const auto block_i = i / block_size;

        const auto block_offset_i = block_i * block_size;

        const auto local_i = i - block_offset_i;

        if (i > 0 && i % block_size == 0)
        {
            for (size_t m = 0; m < 3; ++m)
            {
                std::cout << std::setfill('-') << std::setw(n * 3 + num_blocks - 1) << "-";
                std::cout << "    ";
            }

            std::cout << std::endl;
        }
            
        for (size_t m = 0; m < 3; ++m)
        {
            for (size_t block_j = 0; block_j < num_blocks; ++block_j)
            {
                const auto& block = blocks[block_i][block_j];
                const auto& mat = block[m];

                for (size_t j = 0; j < block_size; ++j)
                {
                    std::cout << std::setfill(' ') << std::setw(2) << mat(local_i, j) << " ";
                }

                if (block_j + 1 == num_blocks) continue;

                std::cout << "|";
            }

            std::cout << "    ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

template<typename T>
void copy_row(matrix<T>& dest, const matrix<T>& src, size_t row)
{
    for (size_t m = 0; m < dest.n(); ++m)
    {
        dest(row, m) = src(row, m);
    }
}

template<typename T>
void copy_col(matrix<T>& dest, const matrix<T>& src, size_t column)
{
    for (size_t m = 0; m < dest.n(); ++m)
    {
        dest(m, column) = src(m, column);
    }
}

template<typename T>
void update_block(std::array<matrix<T>, 3>& block, size_t k)
{
    const auto& a = block[0];
    const auto& b = block[1];
    auto& c = block[2];

    c += compute_outer_product(a, k, b, k);
}

template<typename T>
void update_blocks(std::vector<std::vector<std::array<T, 3>>>& blocks, const size_t k)
{
    for (auto& block_row : blocks)
    {
        for (auto& block : block_row)
        {
            update_block(block, k);
        }
    }
}

template<typename T>
matrix<T> multiply(const matrix<T>& a, const matrix<T>& b, summa_sequential)
{
    Expects(a.n() == b.n());

    auto num_procs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    const auto n = a.n();

    if(my_rank != 0)
        return b;

    const auto blocks_per_dim = get_num_blocks_per_dim(n, 16);

    if(blocks_per_dim == 1)
    {
        return multiply(a, b, naive_sequential());
    }

    const auto block_size = n / blocks_per_dim;

    std::vector<std::vector<std::array<matrix_t, 3>>> blocks;

    blocks.resize(blocks_per_dim);
    for (auto& row : blocks)
    {
        row.resize(blocks_per_dim,
            {
                matrix_t(block_size),
                matrix_t(block_size),
                matrix_t(block_size, 0)
            });
    }

    for (long i = 0; i < n; ++i)
    {
        for (long j = 0; j < n; ++j)
        {
            const auto block_y = i / block_size;
            const auto block_x = j / block_size;
            
            auto& block = blocks[block_y][block_x];
            
            const auto block_offset_x = block_x * block_size;
            const auto local_x = j - block_offset_x;

            const auto block_offset_y = block_y * block_size;
            const auto local_y = i - block_offset_y;
            
            block[0](local_y, local_x) = a(i, j);
            block[1](local_y, local_x) = b(i, j);
        }
    }

    print_blocks(blocks);

    auto old_blocks = blocks;

    for (auto y = gsl::narrow<long>(blocks_per_dim - 1); y >= 0; --y)
    {
        for (auto x = gsl::narrow<long>(blocks_per_dim - 1); x >= 0; --x)
        {
            const auto src_a_y = y % blocks_per_dim;
            const auto src_a_x = (-x - y) % blocks_per_dim;

            const auto src_b_y = (-x - y) % blocks_per_dim;
            const auto src_b_x = x % blocks_per_dim;

            blocks[y][x][0] = old_blocks[src_a_y][src_a_x][0];
            blocks[y][x][1] = old_blocks[src_b_y][src_b_x][1];
        }
    }

    print_blocks(blocks);

    for (size_t k = 0; k < a.n(); ++k)
    { 
        update_blocks(blocks, k % block_size);

        old_blocks = blocks;
                
        for (auto y = gsl::narrow<long>(blocks_per_dim - 1); y >= 0; --y)
        {
            for (auto x = gsl::narrow<long>(blocks_per_dim - 1); x >= 0; --x)
            {
                auto& block = blocks[y][x];
                auto& block_a = block[0];
                auto& block_b = block[1];

                const auto neighbour_x = x == 0
                    ? blocks_per_dim - 1
                    : x - 1;
                     
                const auto neighbour_y = y == 0
                    ? blocks_per_dim - 1
                    : y - 1;

                copy_row(block_b, old_blocks[neighbour_y][x][1], k % block_size);
                copy_col(block_a, old_blocks[y][neighbour_x][0], k % block_size);
            }
        }

        print_blocks(blocks);
    }

    auto c = matrix_t(n);

    for (size_t i = 0; i < n; i += block_size)
    {
        for (size_t j = 0; j < n; j += block_size)
        {
            const auto block_i = i / block_size;
            const auto block_j = j / block_size;

            const auto block_offset_i = block_i * block_size;
            const auto block_offset_j = block_j * block_size;

            auto& block = blocks[block_i][block_j];

            const auto local_i = i - block_offset_i;
            const auto local_j = j - block_offset_j;

            c(i, j) = block[2](local_i, local_j);
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

    template<typename T>
    static matrix<T> create_index_range(size_t n)
    {
        auto m = matrix<T>(n, 0);

        using size_type = typename matrix<T>::size_type;

        auto current = 0;
        for (size_type i = 0; i < n; ++i)
        {
            for (size_type j = 0; j < n; ++j)
            {
                m(i, j) = i * 10 + j;
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
        ? factory_t::create_index_range<value_t>(n)
        : matrix_t(n);

    const auto b = my_rank == 0
       // ? factory_t::create_index_range<value_t>(n)
        ? factory_t::create_identity<value_t>(n)
        : matrix_t(n);

    const auto c = multiply(a, b, technique_t());
        
    MPI_Finalize();

    if(my_rank != 0) return EXIT_SUCCESS;

    const auto diff = duration_cast<milliseconds>(clock::now() - start);

    std::cout << diff.count() << " ms" << std::endl;

    std::cin.get();

    Ensures(c == a);
    
    return EXIT_SUCCESS;
}
