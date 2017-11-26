#include "grid.h"
#include <iostream>
#include "print.h"
#include "grid_helper.h"
#include <string>
#include "chrono_timer.h"

// ==============================================================================
// define types
//using cell_t = float;
using cell_t = std::array<float, 2>;
using cell_value_t = cell_t::value_type;

template<size_t Dim>
using grid_t = stencil::grid_t<cell_t, Dim>;

template<size_t Dim>
using bounds_t = stencil::bounds_t<cell_t, Dim>;

// ==============================================================================
// iteration

template<size_t Dim>
struct jacobi_iteration;

template<>
struct jacobi_iteration<1>
{
    template<size_t ReadIndex>
    static auto execute(grid_t<1>& grid)
    {
        auto error = 0.0f;

        for (auto i = 1u; i < grid.extents()[0] - 1; ++i)
        {
            const auto new_value = (
                grid.at(i - 1)[ReadIndex] + 
                grid.at(i + 1)[ReadIndex]) / 2;

            error += std::abs(new_value - grid.at(i)[ReadIndex]);

            grid.at(i)[1 - ReadIndex] = new_value;
        }

        return error;
    }
};

template<>
struct jacobi_iteration<2>
{
    template<size_t ReadIndex>
    static auto execute(grid_t<2>& grid)
    {
        auto error = 0.0f;

        for (auto y = 1u; y < grid.extents()[1] - 1; ++y)
        {
            for (auto x = 1u; x < grid.extents()[0] - 1; ++x)
            {
                const auto new_value = (
                        grid.at({ x - 1, y + 0 })[ReadIndex] +
                        grid.at({ x + 1, y + 0 })[ReadIndex] +
                        grid.at({ x + 0, y - 1 })[ReadIndex] +
                        grid.at({ x - 0, y + 1 })[ReadIndex]) / 4;

                error += std::abs(new_value - grid.at({x, y})[ReadIndex]);

                grid.at({ x, y })[1 - ReadIndex] = new_value;
            }
        }
            
        return error;
    }
};

template<>
struct jacobi_iteration<3>
{
    template<size_t ReadIndex>
    static auto execute(grid_t<3>& grid)
    {
        auto error = 0.0f;

        for (auto z = 1u; z < grid.extents()[2] - 1; ++z)
        {
            for (auto y = 1u; y < grid.extents()[1] - 1; ++y)
            {
                for (auto x = 1u; x < grid.extents()[0] - 1; ++x)
                {
                    const auto new_value = (
                        grid.at({ x - 1, y + 0, z + 0 })[ReadIndex] +
                        grid.at({ x + 1, y + 0, z + 0 })[ReadIndex] +
                        grid.at({ x + 0, y - 1, z + 0 })[ReadIndex] +
                        grid.at({ x + 0, y + 1, z + 0 })[ReadIndex] +
                        grid.at({ x + 0, y + 0, z - 1 })[ReadIndex] +
                        grid.at({ x + 0, y + 0, z + 1 })[ReadIndex]) / 6;

                    error += std::abs(new_value - grid.at({x, y, z})[ReadIndex]);

                    grid.at({ x, y, z })[1 - ReadIndex] = new_value;
                }
            }
        }

        return error;
    }
};

template<size_t ReadIndex, class GridType>
auto do_jacobi_iteration(GridType& grid)
{
    return jacobi_iteration<GridType::dim>::template execute<ReadIndex>(grid);
}

// ==============================================================================
// entry point

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

template<size_t Dim>
void execute_stencil_code(const float epsilon, 
    const stencil::grid_extents_t<Dim> extents, 
    const bounds_t<Dim> bounds)
{
    chrono_timer timer("create (" + std::to_string(Dim) + "D, " +
        std::to_string(stencil::size(extents)) + ")");

    auto grid = create_grid<cell_t, Dim>(extents, bounds, {0, 0});

    timer.print();  
    timer.stop();

    if (extents[0] < 50)
    {
        std::cout << "\n" << "Start Configuration: " << "\n\n";

        print(grid);
        std::cout << std::endl;
    }

    auto final_value_index = 0;
    auto iterations = 0;

    {
        chrono_timer t("jacobi (" + std::to_string(Dim) + "D, " + 
            std::to_string(stencil::size(extents)) + ")");

        for(;;)
        {
            ++iterations;

            if (do_jacobi_iteration<0>(grid) < epsilon)
            {
                final_value_index = 1;
                break;
            }

            ++iterations;

            if (do_jacobi_iteration<1>(grid) < epsilon)
                break;
        }
    }

    std::cout << "iterations: " << iterations << std::endl;

    // hacky - copies the final values into the first
    // element (for convenience only)
    if (final_value_index == 1) do_jacobi_iteration<1>(grid);

    if (extents[0] > 50) return;

    std::cout << "\n" << "Result: " << "\n\n";
    print(grid);
    std::cout << std::endl;
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

    std::cout << "epsilon: " << epsilon << "\n";
    std::cout << "dim: " << dim << "\n";
    std::cout << "n: " << n << std::endl;

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
