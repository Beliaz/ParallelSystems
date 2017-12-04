#include <cstdlib>
#include "pi.h"
#include <iostream>
#include <string>
#include "../chrono_timer.h"

int main(const int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: pi_par_2 <number_of_samples>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto samples = std::stoull(argv[1]);
    
    {
        ChronoTimer timer("PI_2");

        const auto pi = pi::calculate<pi::method::par_2>(samples);

        std::cout << floor(pi * 100) / 100  << " - ";
    };

    return EXIT_SUCCESS;
}
