#ifndef REQUIREMENTS_H
#define REQUIREMENTS_H

namespace stencil_code
{
    template<class GridType>
    struct view_provider;

    template<class GridType>
    typename view_provider<GridType>::first_view_type 
    get_first(GridType& grid)
    {
        return view_provider<GridType>::get_first(grid);
    }

    template<class GridType>
    typename view_provider<GridType>::second_view_type 
    get_second(GridType& grid)
    {
        return view_provider<GridType>::get_second(grid);
    }

    template<class GridType>
    constexpr auto get_dim()
    {
        return view_provider<GridType>::dim;
    }
}

#endif // REQUIREMENTS_H
