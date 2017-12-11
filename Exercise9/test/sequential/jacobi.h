#ifndef JACOBI_H
#define JACOBI_H

#include "grid.h"

namespace stencil_code
{
    template<size_t Dim>
    struct jacobi;

    template<>
    struct jacobi<1>
    {
        template<class GridViewType>
        static auto execute(
            GridViewType& source,
            const grid_index_t<1>& idx)
        {
            const auto i = idx[0];

            return (
                source.at({ i + 0 }) +
                source.at({ i - 1 }) +
                source.at({ i + 1 })) / 3;
        }
    };

    template<>
    struct jacobi<2>
    {
        template<class GridViewType>
        static auto execute(
            GridViewType& source,
            const grid_index_t<2>& idx)
        {
            const auto x = idx[0];
            const auto y = idx[1];

            return (
                source.at({ x + 0, y + 0 }) +
                source.at({ x - 1, y + 0 }) +
                source.at({ x + 1, y + 0 }) +
                source.at({ x + 0, y - 1 }) +
                source.at({ x - 0, y + 1 })) / 5;
        }
    };

    template<>
    struct jacobi<3>
    {
        template<class GridViewType>
        static auto execute(
            GridViewType& source,
            const grid_index_t<3>& idx)
        {
            const auto x = idx[0];
            const auto y = idx[1];
            const auto z = idx[2];

            return (
                source.at({ x + 0, y + 0, z + 0 }) +
                source.at({ x - 1, y + 0, z + 0 }) +
                source.at({ x + 1, y + 0, z + 0 }) +
                source.at({ x + 0, y - 1, z + 0 }) +
                source.at({ x + 0, y + 1, z + 0 }) +
                source.at({ x + 0, y + 0, z - 1 }) +
                source.at({ x + 0, y + 0, z + 1 })) / 7;
        }
    };
}

#endif // JACOBI_H
