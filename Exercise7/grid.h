#ifndef GRID_H
#define GRID_H

#include <vector>
#include <array>

namespace stencil
{

// =================================================================
// cell

template<class CellType>
struct cell_value;

// =================================================================
// bounds

template<class CellType, size_t Dim>
using bounds_t = std::array<CellType, 2 * Dim>;

// =================================================================
// extents

template<size_t Dim>
using grid_extents_t = std::array<size_t, Dim>;

template<size_t Dim>
auto size(const grid_extents_t<Dim>& extents)
{
    typename grid_extents_t<Dim>::value_type size = 1;

    for (auto i = 0u; i < Dim; ++i)
        size *= extents[i];

    return size;
}

// =================================================================
// index

template<size_t Dim>
using grid_index_t = std::array<size_t, Dim>;

template<size_t Dim>
struct index_helper;

template<>
struct index_helper<1>
{
    static constexpr auto dim = 1;

    static auto linearize(const grid_index_t<dim>& index, const grid_extents_t<dim>& extents)
    {
        return index[0];
    }
};

template<>
struct index_helper<2>
{
    static constexpr auto dim = 2;

    static auto linearize(const grid_index_t<dim>& index, const grid_extents_t<dim>& extents)
    {
        // x + y * width
        return index[0] + extents[0] * index[1];
    }
};

template<>
struct index_helper<3>
{
    static constexpr auto dim = 3;

    static auto linearize(const grid_index_t<dim>& index, const grid_extents_t<dim>& extents)
    {
        // x + y * width + z * width * height
        return index[0] + extents[0] * index[1] + extents[0] * extents[1] * index[2];
    }
};

// =================================================================
// grid

template<class CellType, size_t Dim>
class grid_t
{
public:
    static_assert(Dim > 0 && Dim <= 3, "invalid dimensionality");
    static constexpr auto dim = Dim;

    using cell_type = CellType;
    using storage_type = std::vector<cell_type>;
    using extents_type = grid_extents_t<dim>;
    using index_type = grid_index_t<dim>;
    using index_value_type = typename index_type::value_type;

    explicit grid_t(const extents_type& extents)
        : extents_(extents)
    {
        cells_.reserve(size(extents_));
    }

    const cell_type& at(const index_type& index) const
    {
        return at(index_helper<dim>::linearize(index, extents_));
    }

    cell_type& at(const index_type& index)
    {
        return at(index_helper<dim>::linearize(index, extents_));
    }

    const cell_type& at(const index_value_type linear_index) const
    {
        return cells_[linear_index];
    }

    cell_type& at(const index_value_type linear_index)
    {
        return cells_[linear_index];
    }
    
    const extents_type& extents() const { return extents_; }

private:
    const extents_type extents_;
    storage_type cells_;
};

}


#endif // GRID_H
