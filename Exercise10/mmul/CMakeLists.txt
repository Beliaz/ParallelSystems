cmake_minimum_required(VERSION 2.8.1)
project(mmul)

include(CTest)
enable_testing()
add_subdirectory(${PROJECT_SOURCE_DIR}/test/)

add_executable(mmul mmul.cpp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++latest /wd4141 /arch:AVX2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /arch:AVX2 /DNDEBUG")
else()
	set(CMAKE_C_COMPILER mpicc)
	set(CMAKE_CXX_COMPILER mpicxx)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -march=native -fpermissive")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
endif()

find_package(MPI REQUIRED)
if(MPI_FOUND)
	set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
	set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
	include_directories(${MPI_INCLUDE_PATH})
	target_link_libraries(mmul ${MPI_LIBRARIES})
endif()

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

include_directories(${PROJECT_SOURCE_DIR})
