#include <string>
#include <iostream>
#include <sstream>
#include <numeric>
#include <chrono>

#include "stencil_old/stencil.h"
#include "stencil_old/print.h"
#include "stencil_old/jacobi.h"
#include "stencil_old/iteration.h"

using stencil_code = stencil::stencil_iteration<stencil::jacobi>;

template<size_t Dim>
void execute_stencil_code(const double epsilon, 
    const stencil::grid_extents_t<Dim> extents, 
    const bounds_t<Dim> bounds)
{
    using namespace std::chrono;
    using clock = high_resolution_clock;



    const auto start = clock::now();
    auto times=std::vector<long long int>();
    auto cell_updates_p_second=std::vector<long long int>();
    auto iterations_vetor=std::vector<int>();

    while(true)
    {
        auto grid = create_buffered_grid(extents, bounds, default_value);

        const auto timer = clock::now();
        auto iterations = 0;

        for (;;) {
            const auto result = stencil_code::do_iteration(grid, epsilon);

            iterations += std::get<0>(result);

            if (std::get<1>(result)) break;
        }

        const auto elapsed = duration_cast<milliseconds>(
                clock::now() - timer).count();

        times.push_back(elapsed);


        if(elapsed > 0)
        {
            cell_updates_p_second.push_back(iterations * stencil::size(extents) / static_cast<double>(elapsed) * 1000);
        }
        iterations_vetor.push_back(iterations);

        if(duration_cast<seconds>(clock::now() - start).count() > 10)
            break;

    }

    std::cout << std::accumulate( times.begin(), times.end(), 0.0) / times.size()
              << " ms, " << std::accumulate( iterations_vetor.begin(), iterations_vetor.end(), 0.0) / iterations_vetor.size()
              << " iter, "
              << std::scientific << std::setprecision(3)
              << std::accumulate(cell_updates_p_second.begin(),cell_updates_p_second.end(),0.0) / cell_updates_p_second.size()
              << " cells/s, "
              << iterations_vetor.size() <<" runs";


    std::cout << std::endl;
}


int main(const int argc, char* argv[])
{

#ifdef _MSC_VER
    struct do_finally { ~do_finally() { std::cin.get(); } } _;
#endif

    constexpr auto epsilon = 20;
    constexpr auto n = 512;

    bounds_t<2> bounds;

    bounds[0] = static_cast<std::decay_t<decltype(bounds[0])>>(1.0);
    bounds[1] = static_cast<std::decay_t<decltype(bounds[1])>>(0.5);
    bounds[2] = static_cast<std::decay_t<decltype(bounds[2])>>(0.0);
    bounds[3] = static_cast<std::decay_t<decltype(bounds[3])>>(-0.5);

    try {
        execute_stencil_code<2>(epsilon, {n, n}, bounds);
    } catch(const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
