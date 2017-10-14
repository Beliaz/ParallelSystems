#!/bin/bash

mv ./build/Testing/Temporary/LastTest.log ./bench/benchmark.lastrun.log
./build/googlebenchmark-src/tools/compare_bench.py ./bench/mmul_original.lastrun.json ./bench/mmul_optimized_1.lastrun.json > ./bench/cmp_original_optimization_1.lastrun.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/mmul_original.lastrun.json ./bench/mmul_optimized_2.lastrun.json > ./bench/cmp_original_optimization_2.lastrun.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/mmul_optimized_1.lastrun.json ./bench/mmul_optimized_2.lastrun.json > ./bench/cmp_optimization_1_2.lastrun.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/mmul_original.lastrun.json ./bench/mmul_parallelized.lastrun.json > ./bench/cmp_original_parallelized.lastrun.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/mmul_optimized_2.lastrun.json ./bench/mmul_parallelized.lastrun.json > ./bench/cmp_optimized_2_parallelized.lastrun.txt