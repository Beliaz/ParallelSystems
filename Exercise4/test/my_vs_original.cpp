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
        std::vector<double> _arr = init(atoi(argv[1]));
        std::vector<double> _result =std::vector<double>();
        long my_sort_time;
        long original_sort_time;
        std::cout<<"Num Threads: "<<omp_get_num_threads() <<"   max threads: "<< omp_get_max_threads()<<std::endl;

        {
            ChronoTimer t2("Sort");
            std::sort(_arr.begin(),_arr.end());
            original_sort_time=t2.getTime();
        }
        {
            ChronoTimer t("Sort");
            _result = sort(_arr);
            my_sort_time = t.getTime();
        }
        std::cout<<std::endl<<"My Implementation | std::sort"<<std::endl<<my_sort_time<<" ms          |"<<original_sort_time<<" ms"<<std::endl;
        if (is_sorted(_arr)) return EXIT_SUCCESS;
        else return EXIT_FAILURE;
    } else {
        std::cout<<"Usage: ./merge_sort [num_of_elements]"<<std::endl;
        return EXIT_FAILURE;
    }
}