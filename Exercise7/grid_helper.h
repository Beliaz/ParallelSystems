#ifndef GRID_HELPER_H
#define GRID_HELPER_H

#include "grid.h"

template<size_t Dim>
struct grid_helper;

template<>
struct grid_helper<1>
{
    template<class GridType, class T>
    static void initialize(GridType& grid,
        const stencil::bounds_t<T, 1>& bounds,
        const T& initial_value)
    {
        decltype(auto) first = get_first(grid);
        decltype(auto) second = get_second(grid);

        const auto& extents = first.extents();

        first.at({ 0 }) = bounds[0];
        second.at({ 0 }) = bounds[0];

        first.at({ extents[0] - 1 }) = bounds[1];
        second.at({ extents[0] - 1 }) = bounds[1];

        for (auto i = 1u; i < extents[0] - 1; ++i)
        {
            first.at({ i }) = initial_value;
            second.at({ i }) = initial_value;
        }
    }
};

template<>
struct grid_helper<2>
{
    template<class GridType, class T>
    static void initialize(GridType& grid,
        const stencil::bounds_t<T, 2>& bounds, 
        const T& initial_value)
    {
        decltype(auto) first = get_first(grid);
        decltype(auto) second = get_second(grid);

        const auto& extents = first.extents();

        for (auto y = 0u; y < extents[1]; ++y)
        {
            for (auto x = 0u; x < extents[0]; ++x)
            {
                auto value = initial_value;

                value += x == 0              ? bounds[0] : 0;
                value += x == extents[0] - 1 ? bounds[1] : 0;

                value += y == 0              ? bounds[2] : 0;
                value += y == extents[1] - 1 ? bounds[3] : 0;

                first.at({ x, y }) = value;
                second.at({ x, y}) = value;
            }
        }
    }
};

template<>
struct grid_helper<3>
{
    template<class GridType, class T>
    static void initialize(GridType& grid,
        const stencil::bounds_t<T, 3>& bounds,
        const T& initial_value)
    {
        decltype(auto) first = get_first(grid);
        decltype(auto) second = get_second(grid);

        const auto& extents = first.extents();

        #pragma omp parallel
        for (auto z = 0u; z < extents[2]; ++z)
        {
            #pragma omp for
            for (auto y = 0u; y < extents[1]; ++y)
            {
                for (auto x = 0u; x < extents[0]; ++x)
                {
                    auto value = initial_value;

                    value += x == 0              ? bounds[0] : 0;
                    value += x == extents[0] - 1 ? bounds[1] : 0;

                    value += y == 0              ? bounds[2] : 0;
                    value += y == extents[1] - 1 ? bounds[3] : 0;

                    value += z == 0              ? bounds[4] : 0;
                    value += z == extents[2] - 1 ? bounds[5] : 0;

                    first.at({ x, y, z }) = value;
                    second.at({ x, y, z }) = value;
                }
            }
        }
    }
};

template<class CellType, size_t Dim, class T>
stencil::grid_t<CellType, Dim> create_grid(stencil::grid_extents_t<Dim> extents, 
    const stencil::bounds_t<T, Dim>& bounds,
    const T& initial_value)
{
    for (auto& extent : extents)
        extent += 2; // make place for bounds

    stencil::grid_t<CellType, Dim> grid(extents);

    return grid;
}

template<class GridType,size_t ReadIndex>
class grid_view
{
public:
    using index_type = typename GridType::index_type;

    static constexpr auto dim = GridType::dim;

    explicit grid_view(GridType& grid)
        : grid_(grid)
    {
    }

    auto& at(const index_type& index)
    {
        return grid_.at(index)[ReadIndex];
    }

    const auto& at(const index_type& index) const
    {
        return grid_.at(index)[ReadIndex];
    }

    const auto& extents() const { return grid_.extents(); }

private:
    GridType& grid_;
}; 

template<size_t ReadIndex, class GridType>
auto create_grid_view(GridType& grid)
{
    return grid_view<GridType, ReadIndex>(grid);
}

#endif // GRID_HELPER_H
