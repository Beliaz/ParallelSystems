#ifndef JACOBI_H
#define JACOBI_H

#include "grid.h"
#include <cmath>

namespace stencil
{
    template<size_t Dim>
    struct jacobi;

    template<>
    struct jacobi<1>
    {
        template<class CellType>
        static auto execute(
            grid_t<CellType, 1>& source,
            const size_t i)
        {
            return (
                source.at(i + 0) +
                source.at(i - 1) +
                source.at(i + 1)) / 3;
        }
    };

    template<>
    struct jacobi<2>
    {
        template<class CellType>
        static auto execute(
            grid_t<CellType, 2>& source, 
            const size_t x, 
            const size_t y)
        {
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
        template<class CellType>
        static auto execute(
            grid_t<CellType, 3>& source,
            const size_t x, 
            const size_t y,
            const size_t z)
        {
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
