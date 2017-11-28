#include "../stencil.h"
#include "../jacobi.h"
#include "../iteration.h"

int main(int, char*[])
{
    using namespace stencil;
    using jacobi_3 = jacobi<3>;
    using iteration = iteration<3>;

    constexpr ::bounds_t<3> bounds = { 7, 7, 7, 7, 7, 7 };
    constexpr grid_extents_t<3> extents = { 3, 3, 3 };
    
    auto grid = create_buffered_grid(extents, bounds, default_value);

    decltype(auto) source = get_first(grid);
    decltype(auto) target = get_second(grid);

    if (jacobi_3::execute(source, { 1, 1, 1 }) != 3) return EXIT_FAILURE;
    if (jacobi_3::execute(source, { 2, 1, 1 }) != 2) return EXIT_FAILURE;
    if (jacobi_3::execute(source, { 2, 2, 1 }) != 1) return EXIT_FAILURE;
    if (jacobi_3::execute(source, { 2, 2, 2 }) != 0) return EXIT_FAILURE;

    if(iteration::execute<jacobi>(source, target) != 54) return EXIT_FAILURE;

    if (target.at({ 1, 1, 1 }) != 3) return EXIT_FAILURE;
    if (target.at({ 2, 1, 1 }) != 2) return EXIT_FAILURE;
    if (target.at({ 3, 1, 1 }) != 3) return EXIT_FAILURE;
    
    if (target.at({ 1, 2, 1 }) != 2) return EXIT_FAILURE;
    if (target.at({ 2, 2, 1 }) != 1) return EXIT_FAILURE;
    if (target.at({ 3, 2, 1 }) != 2) return EXIT_FAILURE;

    if (target.at({ 1, 3, 1 }) != 3) return EXIT_FAILURE;
    if (target.at({ 2, 3, 1 }) != 2) return EXIT_FAILURE;
    if (target.at({ 3, 3, 1 }) != 3) return EXIT_FAILURE;

    if (target.at({ 1, 1, 2 }) != 2) return EXIT_FAILURE;
    if (target.at({ 2, 1, 2 }) != 1) return EXIT_FAILURE;
    if (target.at({ 3, 1, 2 }) != 2) return EXIT_FAILURE;

    if (target.at({ 1, 2, 2 }) != 1) return EXIT_FAILURE;
    if (target.at({ 2, 2, 2 }) != 0) return EXIT_FAILURE;
    if (target.at({ 3, 2, 2 }) != 1) return EXIT_FAILURE;

    if (target.at({ 1, 3, 2 }) != 2) return EXIT_FAILURE;
    if (target.at({ 2, 3, 2 }) != 1) return EXIT_FAILURE;
    if (target.at({ 3, 3, 2 }) != 2) return EXIT_FAILURE;

    if (target.at({ 1, 1, 3 }) != 3) return EXIT_FAILURE;
    if (target.at({ 2, 1, 3 }) != 2) return EXIT_FAILURE;
    if (target.at({ 3, 1, 3 }) != 3) return EXIT_FAILURE;

    if (target.at({ 1, 2, 3 }) != 2) return EXIT_FAILURE;
    if (target.at({ 2, 2, 3 }) != 1) return EXIT_FAILURE;
    if (target.at({ 3, 2, 3 }) != 2) return EXIT_FAILURE;

    if (target.at({ 1, 3, 3 }) != 3) return EXIT_FAILURE;
    if (target.at({ 2, 3, 3 }) != 2) return EXIT_FAILURE;
    if (target.at({ 3, 3, 3 }) != 3) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
