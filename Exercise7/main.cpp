#include "grid.h"
#include "print.h"
#include "grid_helper.h"
#include "stencil.h"

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>

template<size_t Dim>
void execute_stencil_code(const float epsilon, 
    const stencil::grid_extents_t<Dim> extents, 
    const bounds_t<Dim> bounds)
{
    using namespace std::chrono;
    using clock = high_resolution_clock;

    auto grid = create_grid<cell_t, Dim>(extents, bounds, {0, 0});

    constexpr auto printing_threshold = 50;

    if (extents[0] < printing_threshold)
    {
        std::cout << "\n" << "Start Configuration: " << "\n\n";

        print(grid);

        std::cout << std::endl;
    }

    std::stringstream ss;
    auto iterations = 0;

    const auto start = clock::now();
    
    for(;;)
    {
        const auto result = stencil_code::do_iteration(grid, epsilon);

        iterations += std::get<0>(result);

        if (std::get<1>(result)) break;
    }

    const auto elapsed = duration_cast<milliseconds>(
        clock::now() - start).count();

    std::cout << elapsed << " ms, " << iterations << " iter";

    if(elapsed > 0)
    {
        std::cout << ", " << std::scientific << std::setprecision(3)
            << iterations * stencil::size(grid.extents()) / static_cast<double>(elapsed) * 1000
            << " cells/s";
    }

    std::cout << std::endl;
    
    if (extents[0] > printing_threshold) return;

    // hacky - copies the final values into the first
    // element as print expects values to be there...
    if (iterations % 2 == 1) stencil_code::do_iteration(grid, epsilon);

    std::cout << "\n" << "Result: " << "\n\n";

    print(grid);

    std::cout << std::endl;
}

template<size_t Dim>
bounds_t<Dim> parse_bounds(const int argc, char* argv[], const size_t offset)
{
    bounds_t<Dim> bounds;

    if (argc < static_cast<int>(offset + bounds.size()))
    {
        throw std::runtime_error("bounds missing ( " +
            std::to_string(argc - offset) +
            " instead of " +
            std::to_string(bounds.size()) +
            " )");
    }

    for (auto i = 0u; i < bounds.size(); ++i)
        bounds[i] = {
        static_cast<cell_value_t>(atof(argv[offset + i])),
        static_cast<cell_value_t>(atof(argv[offset + i]))
    };

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

    const auto epsilon = static_cast<cell_value_t>(atof(argv[1]));
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
        case 1: execute_stencil_code<1>(epsilon, { n }, 
            parse_bounds<1>(argc, argv, bounds_arg_offset)); break;

        case 2: execute_stencil_code<2>(epsilon, { n, n }, 
            parse_bounds<2>(argc, argv, bounds_arg_offset)); break;

        case 3: execute_stencil_code<3>(epsilon, { n, n, n },
            parse_bounds<3>(argc, argv, bounds_arg_offset)); break;

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
