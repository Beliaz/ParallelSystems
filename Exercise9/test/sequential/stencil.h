#ifndef STENCIL_H
#define STENCIL_H

#include "requirements.h"
#include "grid_helper.h"

//#define STRUCTURE_OF_ARRAYS

#ifdef STRUCTURE_OF_ARRAYS

using cell_t = float;

static constexpr cell_t default_value = 0;

template<size_t Dim>
using bounds_t = stencil::bounds_t<cell_t, Dim>;

template<size_t Dim>
using grid_t = stencil::grid_t<cell_t, Dim>;

template<class GridType>
using buffered_grid_t = std::array<GridType, 2>;

namespace stencil
{
    template<class GridType>
    struct view_provider<buffered_grid_t<GridType>>
    {
        static constexpr auto dim = GridType::dim;

        using first_view_type = ::grid_t<dim>&;
        using second_view_type = ::grid_t<dim>&;

        static first_view_type get_first(buffered_grid_t<GridType>& grid)
        {
            return grid[0];
        }

        static second_view_type get_second(buffered_grid_t<GridType>& grid)
        {
            return grid[1];
        }
    };
}

template<size_t Dim, class T>
auto create_buffered_grid(stencil::grid_extents_t<Dim> extents,
    const ::bounds_t<Dim>& bounds,
    const T& initial_value)
{
    auto grid = buffered_grid_t<::grid_t<Dim>>
    {
        create_grid<cell_t, Dim>(extents, bounds, initial_value),
        create_grid<cell_t, Dim>(extents, bounds, initial_value)
    };

    grid_helper<Dim>::initialize(grid, bounds, initial_value);

    return grid;
}

#else

using cell_t = std::array<float, 2>;

static constexpr float default_value = 0;

template<size_t Dim>
using grid_t = stencil::grid_t<cell_t, Dim>;

template<size_t Dim>
using bounds_t = stencil::bounds_t<float, Dim>;

namespace stencil
{
    template<class GridType>
    struct view_provider
    {
        static constexpr auto dim = GridType::dim;

        using first_view_type = grid_view<GridType, 0>;
        using second_view_type = grid_view<GridType, 1>;

        static first_view_type get_first(GridType& grid)
        {
            return create_grid_view<0>(grid);
        }

        static second_view_type get_second(GridType& grid)
        {
            return create_grid_view<1>(grid);
        }
    };
}

template<size_t Dim, class T>
auto create_buffered_grid(stencil::grid_extents_t<Dim> extents,
    const ::bounds_t<Dim>& bounds,
    const T& initial_value)
{
    auto grid = create_grid<cell_t, Dim>(extents, bounds, initial_value);

    grid_helper<Dim>::initialize(grid, bounds, initial_value);

    return grid;
}

#endif

#endif // STENCIL_H
