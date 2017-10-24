#!/bin/bash
cp ./build/Testing/Temporary/LastTest.log ./bench/lastrun/benchmark.log
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/nested_vector.json ./bench/lastrun/contiguous_with_multiplication.json > ./bench/lastrun/cmp_nested_vector_contiguous_with_multiplication.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/nested_vector.json ./bench/lastrun/contiguous_with_indirection.json > ./bench/lastrun/cmp_nested_vector_contiguous_with_indirection.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/nested_vector.json ./bench/lastrun/triangle_matrix.json > ./bench/lastrun/cmp_nested_vector_triangle_matrix.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/nested_vector.json ./bench/lastrun/triangle_matrix_parallel.json > ./bench/lastrun/cmp_nested_vector_triangle_matrix_parallel.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/contiguous_with_multiplication.json ./bench/lastrun/contiguous_with_indirection.json > ./bench/lastrun/cmp_multiplication_indirection.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/contiguous_with_multiplication.json ./bench/lastrun/triangle_matrix.json > ./bench/lastrun/cmp_multiplication_triangle_matrix.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/contiguous_with_multiplication.json ./bench/lastrun/triangle_matrix_parallel.json > ./bench/lastrun/cmp_multiplication_triangle_matrix_parallel.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/contiguous_with_indirection.json ./bench/lastrun/triangle_matrix.json > ./bench/lastrun/cmp_indirection_triangle_matrix.txt
./build/googlebenchmark-src/tools/compare_bench.py ./bench/lastrun/contiguous_with_indirection.json ./bench/lastrun/triangle_matrix_parallel.json > ./bench/lastrun/cmp_indirection_triangle_matrix_parallel.txt