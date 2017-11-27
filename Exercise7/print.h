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

        std::cout << grid.at(0) << " ||";

        for (auto i = 1u; i < grid.extents()[0] - 1; ++i)
        {
            std::cout << " " << grid.at(i) ;
        }

        if(grid.extents()[0] > 1)
            std::cout << " || " << grid.at(grid.extents()[0] - 1);

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

        for (auto y = 0u; y < grid.extents()[0]; ++y)
        {
            if (y == grid.extents()[1] - 1)
            {
                std::cout << "===";

                for (auto x = 0u; x < grid.extents()[1]; ++x)
                    std::cout << "======";

                std::cout << "==";

                std::cout << "\n";
            }
            
            for (auto x = 0u; x < grid.extents()[1]; ++x)
            {
                if (x == grid.extents()[0] - 1)
                    std::cout << "|| ";

                std::cout << grid.at({ x, y }) << " ";

                if (x == 0) std::cout << "|| ";
            }

            if (y == 0)
            {
                std::cout << "\n";

                std::cout << "===";

                for (auto x = 0u; x < grid.extents()[1]; ++x)
                    std::cout << "======";

                std::cout << "==";
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

            for (auto y = 0u; y < grid.extents()[0]; ++y)
            {
                for (auto x = 0u; x < grid.extents()[1]; ++x)
                {
                    std::cout << grid.at({ x, y, z }) << " ";
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
