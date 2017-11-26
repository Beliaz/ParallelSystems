#ifndef GRID_HELPER_H
#define GRID_HELPER_H

#include "grid.h"

template<size_t Dim>
struct grid_helper;

template<>
struct grid_helper<1>
{
    template<class CellType>
    static void initialize(stencil::grid_t<CellType, 1>& grid, 
        const stencil::bounds_t<CellType, 1>& bounds, 
        const CellType& initial_value)
    {
        grid.at(0) = bounds[0];
        grid.at(grid.extents()[0] - 1) = bounds[1];

        for (auto i = 1u; i < grid.extents()[0] - 1; ++i)
            grid.at(i) = initial_value;
    }
};

template<>
struct grid_helper<2>
{
    template<class CellType>
    static void initialize(stencil::grid_t<CellType, 2>& grid, 
        const stencil::bounds_t<CellType, 2>& bounds, 
        const CellType& initial_value)
    {
        const auto& extents = grid.extents();

        for (auto y = 0u; y < grid.extents()[1]; ++y)
        {
            for (auto x = 0u; x < grid.extents()[0]; ++x)
            {
                auto value = initial_value;

                value[0] += x == 0              ? bounds[0][0] : 0;
                value[0] += x == extents[0] - 1 ? bounds[1][0] : 0;

                value[0] += y == 0              ? bounds[2][0] : 0;
                value[0] += y == extents[1] - 1 ? bounds[3][0] : 0;

                value[1] = value[0];

                grid.at({ x, y }) = value;
            }
        }
    }
};

template<>
struct grid_helper<3>
{
    template<class CellType>
    static void initialize(stencil::grid_t<CellType, 3>& grid, 
        const stencil::bounds_t<CellType, 3>& bounds, 
        const CellType& initial_value)
    {
        const auto& extents = grid.extents();

        for (auto z = 0u; z < grid.extents()[2]; ++z)
        {
            for (auto y = 0u; y < grid.extents()[1]; ++y)
            {
                for (auto x = 0u; x < grid.extents()[0]; ++x)
                {
                    auto value = initial_value;

                    value[0] += x == 0              ? bounds[0][0] : 0;
                    value[0] += x == extents[0] - 1 ? bounds[1][0] : 0;

                    value[0] += y == 0              ? bounds[2][0] : 0;
                    value[0] += y == extents[1] - 1 ? bounds[3][0] : 0;

                    value[0] += z == 0              ? bounds[4][0] : 0;
                    value[0] += z == extents[2] - 1 ? bounds[5][0] : 0;

                    value[1] = value[0];

                    grid.at({ x, y, z }) = value;
                }
            }
        }
    }
};

template<class CellType, size_t Dim>
stencil::grid_t<CellType, Dim> create_grid(stencil::grid_extents_t<Dim> extents, 
    const stencil::bounds_t<CellType, Dim>& bounds,
    const CellType& initial_value)
{
    for (auto& extent : extents)
        extent += 2; // make place for bounds

    stencil::grid_t<CellType, Dim> grid(extents);

    grid_helper<Dim>::initialize(grid, bounds, initial_value);

    return grid;
}


#endif // GRID_HELPER_H
