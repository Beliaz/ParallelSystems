#ifndef JACOBI_H
#define JACOBI_H

#include "grid.h"

namespace stencil
{
    template<size_t Dim>
    struct jacobi_iteration;

    template<>
    struct jacobi_iteration<1>
    {
        template<size_t ReadIndex, class CellType>
        static auto execute(grid_t<CellType, 1>& grid,
            const size_t i)
        {
            const auto new_value = (
                grid.at(i - 1)[ReadIndex] +
                grid.at(i + 1)[ReadIndex]) / 2;

            const auto error = std::abs(new_value - grid.at(i)[ReadIndex]);

            grid.at(i)[1 - ReadIndex] = new_value;

            return error;
        }
    };

    template<>
    struct jacobi_iteration<2>
    {
        template<size_t ReadIndex, class CellType>
        static auto execute(grid_t<CellType, 2> &grid, 
            const size_t x, 
            const size_t y)
        {
            const auto new_value = (
                grid.at({ x - 1, y + 0 })[ReadIndex] +
                grid.at({ x + 1, y + 0 })[ReadIndex] +
                grid.at({ x + 0, y - 1 })[ReadIndex] +
                grid.at({ x - 0, y + 1 })[ReadIndex]) / 4;

            const auto error = std::abs(new_value - grid.at({ x, y })[ReadIndex]);

            grid.at({ x, y })[1 - ReadIndex] = new_value;

            return error;
        }
    };

    template<>
    struct jacobi_iteration<3>
    {
        template<size_t ReadIndex, class CellType>
        static auto execute(grid_t<CellType, 3>& grid,
            const size_t x, 
            const size_t y,
            const size_t z)
        {
            const auto new_value = (
                grid.at({ x - 1, y + 0, z + 0 })[ReadIndex] +
                grid.at({ x + 1, y + 0, z + 0 })[ReadIndex] +
                grid.at({ x + 0, y - 1, z + 0 })[ReadIndex] +
                grid.at({ x + 0, y + 1, z + 0 })[ReadIndex] +
                grid.at({ x + 0, y + 0, z - 1 })[ReadIndex] +
                grid.at({ x + 0, y + 0, z + 1 })[ReadIndex]) / 6;

            const auto error = std::abs(new_value - grid.at({ x, y, z })[ReadIndex]);

            grid.at({ x, y, z })[1 - ReadIndex] = new_value;
 
            return error;
        }
    };
}

#endif // JACOBI_H
