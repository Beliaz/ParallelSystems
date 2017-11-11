#include <cstdlib>
#include "pi.h"
#include <iostream>
#include <string>
#include "../../chrono_timer.h"

int main(const int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: pi_par <number_of_samples>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto samples = std::stoull(argv[1]);
    
    const auto pi_approx = [&]()
    {
        ChronoTimer timer("PI_2");
        return pi::calculate<pi::method::par_2>(samples);
    }();

    return EXIT_SUCCESS;
}
