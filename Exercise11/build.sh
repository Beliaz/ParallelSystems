icc *.c -O3 -ipo -opt-matmul -std=c99 -restrict -march=native -openmp -use-intel-headers -qopt-report=1 -o real -lm -g3
