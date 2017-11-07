//
// Created by ivan on 03/11/17.
//

#define PAR_OPT


#include "../merge_sort.h"
#include "../chrono_timer.h"
#include <cstdlib>
#include <iostream>


int main(int, char**)
{
    std::cout<<"Num Threads: " <<omp_get_num_threads() << "   max threads: "<< omp_get_max_threads() << std::endl;
    
    constexpr auto problem_size = 1000;

    auto arr = init(problem_size);
    arr = sort(arr);

    return is_sorted(arr)
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}