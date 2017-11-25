#ifndef PRINT_H
#define PRINT_H
#include "grid.h"
#include <iostream>

template<size_t Dim>
struct grid_printer;

template<>
struct grid_printer<1>
{
    template<class CellType>
    static void print(const stencil::grid_t<CellType, 1>& grid)
    {
        for (auto i = 0u; i < grid.extents()[0]; ++i)
        {
            std::cout << grid.at(i);
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
        for (auto x = 0u; x < grid.extents()[0]; ++x)
        {
            for (auto y = 0u; y < grid.extents()[1]; ++y)
            {
                std::cout << grid.at({ x, y });
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
        for (auto z = 0u; z < grid.extents()[2]; ++z)
        {
            std::cout << "z = " << z << "\n\n";

            for (auto x = 0u; x < grid.extents()[0]; ++x)
            {
                for (auto y = 0u; y < grid.extents()[1]; ++y)
                {
                    std::cout << grid.at({ x, y, z });
                }

                std::cout << "\n";
            }

            std::cout << "\n\n";
        }

        std::cout << std::endl;
    }
};

#endif // PRINT_H
