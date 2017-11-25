#include "grid.h"
#include <iostream>
#include "print.h"

// ==============================================================================
// define types

using cell_t = float;

template<>
void stencil::set_cell_value(cell_t& cell, const float value)
{
    cell = value;
}

template<size_t Dim>
using grid_t = stencil::grid_t<cell_t, Dim>;

template<size_t Dim>
using bounds_t = stencil::bounds_t<cell_t, Dim>;

// ==============================================================================
// iteration
template<size_t Dim>
struct grid_helper;

template<>
struct grid_helper<1>
{
    static void initialize(grid_t<1>& grid, const bounds_t<1>& bounds, const cell_t& initial_value)
    {
        grid.set({ 0 }, bounds[0]);
        grid.set({ grid.extents()[0] - 1 }, bounds[1]);
        
        for (auto i = 1u; i < grid.extents()[0] - 1; ++i)
            grid.set({ i }, initial_value);
    }
};

template<>
struct grid_helper<2>
{
    static void initialize(grid_t<2>& grid, const bounds_t<2>& bounds, const cell_t& initial_value)
    {
        const auto& extents = grid.extents();

        for (auto y = 0u; y < grid.extents()[1]; ++y)
        {
            for (auto x = 0u; x < grid.extents()[0]; ++x)
            {
                auto value = initial_value;

                value += x == 0 || x == extents[0] - 1 ? bounds[0] : 0;
                value += y == 0 || y == extents[1] - 1 ? bounds[1] : 0;

                grid.set({ x, y }, value);
            }
        }
    }
};

template<>
struct grid_helper<3>
{
    static void initialize(grid_t<3>& grid, const bounds_t<3>& bounds, const cell_t& initial_value)
    {
        const auto& extents = grid.extents();

        for (auto z = 0u; z < grid.extents()[2]; ++z)
        {
            for (auto y = 0u; y < grid.extents()[1]; ++y)
            {
                for (auto x = 0u; x < grid.extents()[0]; ++x)
                {
                    auto value = initial_value;

                    value += x == 0 || x == extents[0] - 1 ? bounds[0] : 0;
                    value += y == 0 || y == extents[1] - 1 ? bounds[1] : 0;
                    value += z == 0 || z == extents[2] - 1 ? bounds[2] : 0;

                    grid.set({ x, y, z }, value);
                }
            }
        }
    }
};

template<size_t Dim>
grid_t<Dim> create_grid(stencil::grid_extents_t<Dim> extents, const bounds_t<Dim>& bounds, const cell_t& initial_value)
{
    for (auto& extent : extents)
        extent += 2; // make place for bounds

    grid_t<Dim> grid(extents);

    grid_helper<Dim>::initialize(grid, bounds, initial_value);

    return grid;
}

template<size_t Dim>
struct jacobi_iteration;

template<>
struct jacobi_iteration<1>
{
    static void execute(double epsilon, const stencil::grid_extents_t<1> extents, const bounds_t<1> bounds)
    {
        auto grid = create_grid<1>(extents, bounds, 0);

        // do iteration

        grid_printer<1>::print(grid);
    }
};

template<>
struct jacobi_iteration<2>
{
    static void execute(double epsilon, const stencil::grid_extents_t<2> extents, const bounds_t<2> bounds)
    {
        auto grid = create_grid<2>(extents, bounds, 0);

        // do iteration

        grid_printer<2>::print(grid);
    }
};

template<>
struct jacobi_iteration<3>
{
    static void execute(double epsilon, const stencil::grid_extents_t<3> extents, const bounds_t<3> bounds)
    {
        auto grid = create_grid<3>(extents, bounds, 0);

        // do iteration

        grid_printer<3>::print(grid);
    }
};

// ==============================================================================
// entry point

template<size_t Dim>
bounds_t<Dim> parse_bounds(char* argv[], const size_t offset)
{
    bounds_t<Dim> bounds;

    for (auto i = 0u; i < bounds.size(); ++i)
        bounds[i] = static_cast<cell_t>(atof(argv[offset + i]));

    return bounds;
}

int main(const int argc, char* argv[])
{
#ifdef _MSC_VER
    struct do_finally { ~do_finally() { std::cin.get(); } } _;
#endif

    if (argc < 6 )
    {
        std::cout << "usage: <epsilon> <dim> <n> <left_bound> <right_bound>" 
                  << "[<top_bound> <bottom_bound> [<front_bound> <back_bound>]]" 
                  << std::endl;

        return EXIT_FAILURE;
    }

    const auto epsilon = atof(argv[1]);
    const auto dim = atoi(argv[2]);
    const auto n = static_cast<size_t>(atoi(argv[3]));

    if(epsilon <= 0)
    {
        std::cerr << "epsilon must be greater than 0";
        return EXIT_FAILURE;
    }

    if (n <= 0)
    {
        std::cerr << "n must be greater than 0";
        return EXIT_FAILURE;
    }

    if(dim == 0 || dim > 3)
    {
        std::cerr << "dim must be either 1, 2 or 3";
        return EXIT_FAILURE;
    }

    constexpr auto bounds_arg_offset = 4;

    try
    {
        switch (dim)
        {
        case 1: jacobi_iteration<1>::execute(epsilon, { n },       
            parse_bounds<1>(argv, bounds_arg_offset)); break;

        case 2: jacobi_iteration<2>::execute(epsilon, { n, n },
            parse_bounds<2>(argv, bounds_arg_offset)); break;

        case 3: jacobi_iteration<3>::execute(epsilon, { n, n, n },
            parse_bounds<3>(argv, bounds_arg_offset)); break;

        default: return EXIT_FAILURE;
        }
    }
    catch(const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
