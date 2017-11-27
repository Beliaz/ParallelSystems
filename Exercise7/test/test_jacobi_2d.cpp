#include "../grid_helper.h"
#include "../jacobi.h"
#include "../iteration.h"
#include "../stencil.h"

int main(int, char*[])
{
    using namespace stencil;
    using jacobi_2 = jacobi<2>;
    using iteration = iteration<2>;

    constexpr ::bounds_t<2> bounds = { 5, 5, 5, 5 };
    constexpr grid_extents_t<2> extents = { 3, 3 };
    constexpr auto default_value = 0;
    
    auto grid = create_grid<cell_t, 2>(extents, bounds, default_value);

    if (jacobi_2::execute(grid, { 1, 1 }) != 2) return EXIT_FAILURE;
    if (jacobi_2::execute(grid, { 2, 1 }) != 1) return EXIT_FAILURE;
    if (jacobi_2::execute(grid, { 2, 2 }) != 0) return EXIT_FAILURE;

    auto grid_2 = create_grid<cell_t, 2>(extents, bounds, default_value);

    if(iteration::execute<jacobi>(grid, grid_2) != 12) return EXIT_FAILURE;

    if (grid_2.at({ 1, 1 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 2, 1 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 3, 1 }) != 2) return EXIT_FAILURE;
    
    if (grid_2.at({ 1, 2 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 2, 2 }) != 0) return EXIT_FAILURE;
    if (grid_2.at({ 3, 2 }) != 1) return EXIT_FAILURE;

    if (grid_2.at({ 1, 3 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 2, 3 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 3, 3 }) != 2) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
