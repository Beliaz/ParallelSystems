cmake_minimum_required(VERSION 3.5.1)
project(Exercise8 LANGUAGES CXX)

# cmake module directory
#set(CMAKE_MODULE_PATH
#        ${CMAKE_MODULE_PATH}
#        ${PROJECT_SOURCE_DIR}/cmake)

#include(CTest)
#enable_testing()
#add_subdirectory(${PROJECT_SOURCE_DIR}/test/)

add_executable(mpi main.cpp)

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

find_package(MPI REQUIRED)
if(MPI_FOUND)
	set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
	set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
	include_directories(MPI_INCLUDE_PATH)
	target_link_libraries(mpi ${MPI_LIBRARIES}) 
endif()

set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")



