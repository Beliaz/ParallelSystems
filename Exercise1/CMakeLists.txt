cmake_minimum_required(VERSION 3.6)
project(Exercise1 LANGUAGES CXX)

# cmake module directory
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake)

# This include is required if you want 
# to use CTest framework for you benchmarks with add_test()
#
include(CTest) 

enable_testing()

set(BENCHMARK_ENABLE_TESTING OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX")
  set(CMAKE_CXX_FLAGS_DEBUG "/O0")
  set(CMAKE_CXX_FLAGS_RELEASE "/O3 /DNDEBUG")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

include(ConfigGBench)

add_subdirectory(${PROJECT_SOURCE_DIR}/test/)
add_subdirectory(${PROJECT_SOURCE_DIR}/bench/)

set(SOURCE_FILES main.cpp)

add_executable(Exercise1 ${SOURCE_FILES})
