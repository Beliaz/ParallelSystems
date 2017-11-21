//
// Created by ivan on 16/11/17.
//

#define PARALLEL_STABLE

#include "../avl.h"
#include <chrono>
#include "../chrono_timer.h"
#include <random>
#include <stdlib.h>


int main(int, char**) 
{
    for (const auto n : { 1000, 1024, 2000, 2048, 8000, 8192 })
    {
        std::default_random_engine random_engine;
        std::uniform_int_distribution<unsigned int> range(0, n / 8);

        random_engine.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

        std::vector<unsigned int> list(n, 0);
        generate(list.begin(), list.end(), [&]() { return range(random_engine); });

        std::cout << "n: " << n << "\n";

        std::cout << "Insert Par_stable" << std::endl;

        avl_tree avl;
        {
            ChronoTimer h("insert");
            avl.insert(list);
        }

        const auto valid = avl.is_valid();

        std::cout << "ordered: " << (valid ? "true" : "false") << std::endl;
        std::cout << std::endl;

        if (!valid) return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}