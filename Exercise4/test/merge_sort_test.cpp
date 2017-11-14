//
// Created by ivan on 03/11/17.
//

#include "../merge_sort.h"
#include "../chrono_timer.h"
#include <cstdlib>

int main(int, char**)
{
    constexpr auto problem_size = 1'000'000;

    auto arr = init(problem_size);
    sort(&arr);
    std::cout<<"res: "<<arr.size()<<" sort: "<<(is_sorted(arr) ? "true":"false")<<" original: "<<problem_size<<std::endl;

    return is_sorted(arr)
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}