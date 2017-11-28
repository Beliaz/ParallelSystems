#include "../grid_helper.h"
#include "../jacobi.h"
#include "../iteration.h"
#include "../stencil.h"

int main(int, char*[])
{
    using namespace stencil;
    using jacobi_1 = jacobi<1>;
    using iteration = iteration<1>;

    constexpr ::bounds_t<1> bounds = { 3, 0 };
    constexpr grid_extents_t<1> extents = { 3 };
  
    auto grid = create_buffered_grid(extents, bounds, default_value);

    decltype(auto) source = get_first(grid);
    decltype(auto) target = get_second(grid);

    if (jacobi_1::execute(source, { 1 }) != 1) return EXIT_FAILURE;
    if (jacobi_1::execute(source, { 2 }) != 0) return EXIT_FAILURE;
    if (jacobi_1::execute(source, { 3 }) != 0) return EXIT_FAILURE;
    
    if(iteration::execute<jacobi>(source, target) != 1) return EXIT_FAILURE;

    if (target.at({ 1 }) != 1) return EXIT_FAILURE;
    if (target.at({ 2 }) != 0) return EXIT_FAILURE;
    if (target.at({ 3 }) != 0) return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}
