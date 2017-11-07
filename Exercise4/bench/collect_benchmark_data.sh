#!/bin/bash

mv ../build/Testing/Temporary/LastTest.log ./benchmark.lastrun.log
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_original.lastrun.json mmul_optimized_1.lastrun.json > ./cmp_original_optimized_1.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_original.lastrun.json ./mmul_optimized_2.lastrun.json > ./cmp_original_optimized_2.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_original.lastrun.json ./mmul_optimized_3.lastrun.json > ./cmp_original_optimized_3.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_original.lastrun.json ./mmul_optimized_4.lastrun.json > ./cmp_original_optimized_4.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_original.lastrun.json ./mmul_parallelized.lastrun.json > ./cmp_original_parallelized.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_1.lastrun.json ./mmul_optimized_2.lastrun.json > ./cmp_optimized_1_2.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_1.lastrun.json ./mmul_optimized_3.lastrun.json > ./cmp_optimized_1_3.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_1.lastrun.json ./mmul_optimized_4.lastrun.json > ./cmp_optimized_1_4.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_2.lastrun.json ./mmul_optimized_3.lastrun.json > ./cmp_optimized_2_3.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_2.lastrun.json ./mmul_optimized_4.lastrun.json > ./cmp_optimized_2_4.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_3.lastrun.json ./mmul_optimized_4.lastrun.json > ./cmp_optimized_3_4.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_2.lastrun.json ./mmul_parallelized.lastrun.json > ./cmp_optimized_2_parallelized.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_3.lastrun.json ./mmul_parallelized.lastrun.json > ./cmp_optimized_3_parallelized.lastrun.txt
../build/googlebenchmark-src/tools/compare_bench.py ./mmul_optimized_4.lastrun.json ./mmul_parallelized.lastrun.json > ./cmp_optimized_4_parallelized.lastrun.txt