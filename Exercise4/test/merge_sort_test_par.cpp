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
    
    constexpr auto problem_size = 100000;

    auto arr = init(problem_size);

    {
        ChronoTimer t("Start");
        sort(&arr);
    }

    std::cout<<"res: "<<arr.size()<<" sort: "<<(is_sorted(arr) ? "true":"false")<<" original: "<<problem_size<<std::endl;


    return is_sorted(arr)
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}