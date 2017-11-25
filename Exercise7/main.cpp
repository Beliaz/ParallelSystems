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

template<size_t Dim>
bounds_t<Dim> parse_bounds(char* argv[], const size_t offset)
{
    bounds_t<Dim> bounds;

    for (auto i = 0u; i < bounds.size(); ++i)
        bounds[i] = static_cast<cell_t>(atof(argv[offset + i]));

    return bounds;
}

// ==============================================================================
// iteration

template<size_t Dim>
struct jacobian_iteration;

template<>
struct jacobian_iteration<1>
{
    static void execute(double epsilon, const stencil::grid_extents_t<1> extents, bounds_t<1> bounds)
    {
        using grid_t = grid_t<1>;

        grid_t grid(extents);

        grid.set({ extents[0] / 2 }, 2);

        // do iteration

        grid_printer<1>::print(grid);
    }
};

template<>
struct jacobian_iteration<2>
{
    static void execute(double epsilon, const stencil::grid_extents_t<2> extents, bounds_t<2> bounds)
    {
        using grid_t = grid_t<2>;

        grid_t grid(extents);

        grid.set({ extents[0] / 2, extents[1] / 2 }, 2);

        // do iteration

        grid_printer<2>::print(grid);
    }
};

template<>
struct jacobian_iteration<3>
{
    static void execute(double epsilon, const stencil::grid_extents_t<3> extents, bounds_t<3> bounds)
    {
        using grid_t = grid_t<3>;

        grid_t grid(extents);

        grid.set({ extents[0] / 2, extents[1] / 2, extents[2] / 2 }, 2);

        // do iteration

        grid_printer<3>::print(grid);
    }
};

// ==============================================================================

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
        case 1: jacobian_iteration<1>::execute(epsilon, { n },       
            parse_bounds<1>(argv, bounds_arg_offset)); break;

        case 2: jacobian_iteration<2>::execute(epsilon, { n, n },
            parse_bounds<2>(argv, bounds_arg_offset)); break;

        case 3: jacobian_iteration<3>::execute(epsilon, { n, n, n },
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
