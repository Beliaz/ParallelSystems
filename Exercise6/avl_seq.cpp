//
// Created by ivan on 16/11/17.
//

#include "avl.h"
#include <string>
#include <chrono>
#include "chrono_timer.h"
#include <random>
#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv) {
    //Do some testing whether the given input is correct.
    if (argc != 2) {
        printf("Please give exactly one problem size");
        return EXIT_FAILURE;

    }

    unsigned int N = std::atoi(argv[1]);

    if (N<20) {
        printf("Please greater than 20");
        return EXIT_FAILURE;

    }

    std::default_random_engine random_engine;
    std::uniform_int_distribution<unsigned int> range(0, N/8);

    random_engine.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    std::vector<unsigned int> list;

    for(unsigned int i=0;i<N;i++)
        list.push_back(range(random_engine));

    std::cout << "Insert Seq" << std::endl;

    avl_tree avl;
    {  
        ChronoTimer h("insert");
        avl.insert(list);
    }

    std::cout<<std::endl<<((avl.check_order()==true)?"true":"false")<<std::endl;

}