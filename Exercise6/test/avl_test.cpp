//
// Created by ivan on 16/11/17.
//

#include "../avl.h"
#include <string>
#include <omp.h>
#include <chrono>
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

    std::default_random_engine random_engine;
    std::uniform_int_distribution<unsigned int> range(0, N/8);

    random_engine.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    std::vector<unsigned int> list;

    for(unsigned int i=0;i<N;i++)
        list.push_back(range(random_engine));

    avlTree avl;
    avl.insert(list);

    std::cout<<"ha"<<std::endl;
}