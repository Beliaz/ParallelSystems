#include <cstdlib>
#include "pi.h"
#include <iostream>
#include <string>

int main(const int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: pi_par <number_of_samples>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto samples = std::stoull(argv[1]);
    const auto pi_approx = pi::calculate<pi::method::par_2>(samples);

    std::cout << "result using method::par_2 and " << samples << " samples : " << pi_approx << std::endl;

    std::cin.get();

    return EXIT_SUCCESS;
}