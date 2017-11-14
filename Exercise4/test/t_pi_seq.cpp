#include "../src/pi/pi.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

int main(int, char**)
{
    constexpr auto samples = 10'000'000;

    const auto estimated_pi = pi::calculate<pi::method::seq_3>(samples);

    std::cout << estimated_pi << std::endl;

    return round(estimated_pi * 10) == round(M_PI * 10)
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
