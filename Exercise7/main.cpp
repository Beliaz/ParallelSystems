#include "grid.h"
#include <iostream>

using cell_t = float;

template<>
struct cell_value<float>
{
    using value_type = float;
};

template<size_t Dim>
using bounds_t = std::array<cell_t, 2 * Dim>;

template<size_t Dim>
bounds_t<Dim> parse_bounds(char* argv[], const size_t offset)
{
    bounds_t<Dim> bounds;

    for (auto i = 0u; i < bounds.size(); ++i)
        bounds[i] = static_cast<cell_t>(atof(argv[offset + i]));

    return bounds;
}

template<size_t Dim>
struct jacobian_iteration;

template<>
struct jacobian_iteration<1>
{
    static void execute(double epsilon, grid_extents_t<1> extents, bounds_t<1> bounds)
    {
        using grid_t = grid_t<cell_t, 1>;

        grid_t grid(extents);

        for (auto i = 0u; i < extents[0]; ++i)
        {
            std::cout << grid.at(i);
        }

        std::cout << std::endl;
    }
};

template<>
struct jacobian_iteration<2>
{
    static void execute(double epsilon, grid_extents_t<2> extents, bounds_t<2> bounds)
    {
        using grid_t = grid_t<cell_t, 2>;

        grid_t grid(extents);

        for (auto x = 0u; x < extents[0]; ++x)
        {
            for(auto y = 0u; y < extents[1]; ++y)
            {
                std::cout << grid.at({x, y});
            }

            std::cout << "\n";
        }

        std::cout << std::endl;
    }
};

template<>
struct jacobian_iteration<3>
{
    static void execute(double epsilon, grid_extents_t<3> extents, bounds_t<3> bounds)
    {
        using grid_t = grid_t<cell_t, 3>;

        grid_t grid(extents);

        for (auto z = 0u; z < extents[2]; ++z)
        {
            std::cout << "z = " << z << "\n\n";

            for (auto x = 0u; x < extents[0]; ++x)
            {
                for (auto y = 0u; y < extents[1]; ++y)
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


int main(const int argc, char* argv[])
{
    struct do_finally { ~do_finally() { std::cin.get(); } } _;

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
