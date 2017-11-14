cmake_minimum_required(VERSION 3.5.2)
project(Exercise4 LANGUAGES CXX)

# cmake module directory
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake)


include(CTest)

enable_testing()

set(BENCHMARK_ENABLE_TESTING OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX /arch:AVX2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /arch:AVX2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
endif()


FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

include(ConfigGBench)

add_subdirectory(${PROJECT_SOURCE_DIR}/test/)
add_subdirectory(${PROJECT_SOURCE_DIR}/bench/)

aux_source_directory(. SRC_LIST)

add_executable(pi_par src/pi/pi_par.cpp)
add_executable(pi_par_2 src/pi/pi_par_2.cpp)
add_executable(pi_seq src/pi/pi_seq.cpp)
