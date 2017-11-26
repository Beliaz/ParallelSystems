#ifndef PRINT_H
#define PRINT_H
#include "grid.h"
#include <iostream>
#include <iomanip>

template<size_t Dim>
struct grid_printer;

template<>
struct grid_printer<1>
{
    template<class CellType>
    static void print(const stencil::grid_t<CellType, 1>& grid)
    {
        std::cout << std::fixed << std::setprecision(3);

        for (auto i = 0u; i < grid.extents()[0]; ++i)
        {
            std::cout << grid.at(i)[0] << " ";
        }

        std::cout << std::endl;
    }
};

template<>
struct grid_printer<2>
{
    template<class CellType>
    static void print(const stencil::grid_t<CellType, 2>& grid)
    {
        std::cout << std::fixed << std::setprecision(3);

        for (auto x = 0u; x < grid.extents()[0]; ++x)
        {
            for (auto y = 0u; y < grid.extents()[1]; ++y)
            {
                std::cout << grid.at({ x, y })[0] << " ";
            }

            std::cout << "\n";
        }
    }
};

template<>
struct grid_printer<3>
{
    template<class CellType>
    static void print(const stencil::grid_t<CellType, 3>& grid)
    {
        std::cout << std::fixed << std::setprecision(3);

        for (auto z = 0u; z < grid.extents()[2]; ++z)
        {
            std::cout << "z = " << z << "\n\n";

            for (auto x = 0u; x < grid.extents()[0]; ++x)
            {
                for (auto y = 0u; y < grid.extents()[1]; ++y)
                {
                    std::cout << grid.at({ x, y, z })[0] << " ";
                }

                std::cout << "\n";
            }

            std::cout << "\n\n";
        }

        std::cout << std::endl;
    }
};

template<class GridType>
void print(const GridType& grid)
{
    grid_printer<GridType::dim>::print(grid);
}

#endif // PRINT_H
