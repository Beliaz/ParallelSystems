icc *.c -O3 -ipo -opt-matmul -parallel -openmp -std=c99 -restrict -march=native -use-intel-headers -qopt-report=5 -o real -lm -g3
