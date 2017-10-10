cmake_minimum_required(VERSION 3.6)
project(ParallelSystemes)

set(CMAKE_C_COMPILER "CXX=g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES Exercise1/mmul.cpp)
add_executable(Exercise1 ${SOURCE_FILES})
