#include "../grid_helper.h"
#include "../jacobi.h"
#include "../iteration.h"
#include "../stencil.h"

int main(int, char*[])
{
    using namespace stencil;
    using jacobi_1 = jacobi<1>;
    using iteration = iteration<1>;

    auto grid = create_grid<cell_t, 1>({ 3 }, { 3, 0 }, 0);

    if (jacobi_1::execute(grid, { 1 }) != 1) return EXIT_FAILURE;
    if (jacobi_1::execute(grid, { 2 }) != 0) return EXIT_FAILURE;
    if (jacobi_1::execute(grid, { 3 }) != 0) return EXIT_FAILURE;
    
    auto grid_2 = create_grid<cell_t, 1>({ 3 }, { 3, 0 }, 0);

    if(iteration::execute<jacobi>(grid, grid_2) != 1) return EXIT_FAILURE;

    if (grid_2.at(1) != 1) return EXIT_FAILURE;
    if (grid_2.at(2) != 0) return EXIT_FAILURE;
    if (grid_2.at(3) != 0) return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}
