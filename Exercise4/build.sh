module load gcc/5.1.0

g++ -std=c++14 -O3 -msse4.1 -fopenmp -o ./build/pi_par_1_gcc ./src/pi/pi_par.cpp
g++ -std=c++14 -O3 -msse4.1 -fopenmp -o ./build/pi_par_2_gcc ./src/pi/pi_par_2.cpp

module unload gcc/5.1.0

module load intel/15.0

icpc -std=c++14 -O3 -xSSE4.1 -openmp -o ./build/pi_par_1_icc ./src/pi/pi_par.cpp
icpc -std=c++14 -O3 -xSSE4.1 -openmp -o ./build/pi_par_2_icc ./src/pi/pi_par_2.cpp

module unload intel/15.0

module load pgi/17.7

pgc++ -openmp -fastsse -std=c++11 -o ./build/pi_par_1_pgi src/pi/pi_par.cpp
pgc++ -openmp -fastsse -std=c++11 -o ./build/pi_par_2_pgi src/pi/pi_par_2.cpp

module unload pgi/17.7
