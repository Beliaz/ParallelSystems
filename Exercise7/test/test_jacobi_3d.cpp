#include "../grid_helper.h"
#include "../jacobi.h"
#include "../iteration.h"
#include "../stencil.h"

int main(int, char*[])
{
    using namespace stencil;
    using jacobi_3 = jacobi<3>;
    using iteration = iteration<3>;

    constexpr ::bounds_t<3> bounds = { 7, 7, 7, 7, 7, 7 };
    constexpr grid_extents_t<3> extents = { 3, 3, 3 };
    constexpr auto default_value = 0;
    
    auto grid = create_grid<cell_t, 3>(extents, bounds, default_value);

    if (jacobi_3::execute(grid, { 1, 1, 1 }) != 3) return EXIT_FAILURE;
    if (jacobi_3::execute(grid, { 2, 1, 1 }) != 2) return EXIT_FAILURE;
    if (jacobi_3::execute(grid, { 2, 2, 1 }) != 1) return EXIT_FAILURE;
    if (jacobi_3::execute(grid, { 2, 2, 2 }) != 0) return EXIT_FAILURE;

    auto grid_2 = create_grid<cell_t, 3>(extents, bounds, default_value);

    if(iteration::execute<jacobi>(grid, grid_2) != 54) return EXIT_FAILURE;

    if (grid_2.at({ 1, 1, 1 }) != 3) return EXIT_FAILURE;
    if (grid_2.at({ 2, 1, 1 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 3, 1, 1 }) != 3) return EXIT_FAILURE;
    
    if (grid_2.at({ 1, 2, 1 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 2, 2, 1 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 3, 2, 1 }) != 2) return EXIT_FAILURE;

    if (grid_2.at({ 1, 3, 1 }) != 3) return EXIT_FAILURE;
    if (grid_2.at({ 2, 3, 1 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 3, 3, 1 }) != 3) return EXIT_FAILURE;

    if (grid_2.at({ 1, 1, 2 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 2, 1, 2 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 3, 1, 2 }) != 2) return EXIT_FAILURE;

    if (grid_2.at({ 1, 2, 2 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 2, 2, 2 }) != 0) return EXIT_FAILURE;
    if (grid_2.at({ 3, 2, 2 }) != 1) return EXIT_FAILURE;

    if (grid_2.at({ 1, 3, 2 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 2, 3, 2 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 3, 3, 2 }) != 2) return EXIT_FAILURE;

    if (grid_2.at({ 1, 1, 3 }) != 3) return EXIT_FAILURE;
    if (grid_2.at({ 2, 1, 3 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 3, 1, 3 }) != 3) return EXIT_FAILURE;

    if (grid_2.at({ 1, 2, 3 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 2, 2, 3 }) != 1) return EXIT_FAILURE;
    if (grid_2.at({ 3, 2, 3 }) != 2) return EXIT_FAILURE;

    if (grid_2.at({ 1, 3, 3 }) != 3) return EXIT_FAILURE;
    if (grid_2.at({ 2, 3, 3 }) != 2) return EXIT_FAILURE;
    if (grid_2.at({ 3, 3, 3 }) != 3) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
