//
// Created by ivan on 03/11/17.
//

#include "../merge_sort.h"
#include "../chrono_timer.h"
#include <cstdlib>

#define PAR_OPT

int main(int args, char** argv){

    std::vector<double> _arr = init(atoi(argv[1]));
    {
        ChronoTimer t("Sort");
        _arr = sort(_arr);
    }
    if(is_sorted(_arr)) return EXIT_SUCCESS;
    else return EXIT_FAILURE;
}