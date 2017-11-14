//
// Created by ivan on 03/11/17.
//

#define PAR_OPT


#include "../merge_sort.h"
#include "../chrono_timer.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>


int main(int args, char** argv){

    if(args==2) {
        auto _arr = init(atoi(argv[1]));
        auto _arr1 = _arr;
        auto _arr2 = _arr;
        long long my_sort_time;
        long long original_sort_time;
        long long merge_sort_time;
        std::cout<<"Num Threads: "<<omp_get_num_threads() <<"   max threads: "<< omp_get_max_threads()<<std::endl;

        {
            ChronoTimer t("My merge sort");
            sort(&_arr);
            my_sort_time = t.getTime();
        }
        {
            ChronoTimer t2("Std::sort");
            std::sort(_arr1.begin(),_arr1.end());
            original_sort_time=t2.getTime();
        }
        {
            ChronoTimer t3("std::stable_sort");
            std::stable_sort(_arr2.begin(),_arr2.end());
            merge_sort_time = t3.getTime();
        }

        std::cout<<std::endl<<"My Implementation | std::sort | std::stable_sort"<<std::endl;
        std::cout<<my_sort_time<<((my_sort_time<1000)?" ":"")<<" ms           | "<<original_sort_time<<" ms    | "<<merge_sort_time<<" ms"<<std::endl;
        if (is_sorted(_arr) && is_sorted(_arr1) && is_sorted(_arr2)) return EXIT_SUCCESS;
        else return EXIT_FAILURE;
    } else {
        std::cout<<"Usage: ./merge_sort [num_of_elements]"<<std::endl;
        return EXIT_FAILURE;
    }
}