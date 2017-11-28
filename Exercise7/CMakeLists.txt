cmake_minimum_required(VERSION 2.8.12)
project(Exercise7 LANGUAGES CXX)

# cmake module directory
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake)

include(CTest)

enable_testing()

set(BENCHMARK_ENABLE_TESTING OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /arch:AVX2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /arch:AVX2 /DNDEBUG")
else()
	set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -march=native -fpermissive")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
endif()


FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

include(ConfigGBench)

add_subdirectory(${PROJECT_SOURCE_DIR}/bench/)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/)

aux_source_directory(. SRC_LIST)

add_executable(stencil_1 main.cpp)
add_executable(stencil_2 stencil_code.cpp)
