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
    
    auto grid = create_buffered_grid(extents, bounds, default_value);

    decltype(auto) source = get_first(grid);
    decltype(auto) target = get_second(grid);

    if (jacobi_2::execute(source, { 1, 1 }) != 2) return EXIT_FAILURE;
    if (jacobi_2::execute(source, { 2, 1 }) != 1) return EXIT_FAILURE;
    if (jacobi_2::execute(source, { 2, 2 }) != 0) return EXIT_FAILURE;

    if(iteration::execute<jacobi>(source, target) != 12) return EXIT_FAILURE;

    if (target.at({ 1, 1 }) != 2) return EXIT_FAILURE;
    if (target.at({ 2, 1 }) != 1) return EXIT_FAILURE;
    if (target.at({ 3, 1 }) != 2) return EXIT_FAILURE;
    
    if (target.at({ 1, 2 }) != 1) return EXIT_FAILURE;
    if (target.at({ 2, 2 }) != 0) return EXIT_FAILURE;
    if (target.at({ 3, 2 }) != 1) return EXIT_FAILURE;

    if (target.at({ 1, 3 }) != 2) return EXIT_FAILURE;
    if (target.at({ 2, 3 }) != 1) return EXIT_FAILURE;
    if (target.at({ 3, 3 }) != 2) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
