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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /DNDEBUG")
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

add_executable(merge_sort test/merge_sort_test.cpp)
target_link_libraries(merge_sort ${SRC_LIST})

add_executable(merge_sort_par test/merge_sort_test_par.cpp)
target_link_libraries(merge_sort ${SRC_LIST})

add_executable(merge_sort_seq test/merge_sort_test_seq.cpp)
target_link_libraries(merge_sort ${SRC_LIST})

add_executable(my_vs_std src/my_vs_original.cpp)
target_link_libraries(my_vs_std ${SRC_LIST})

#add_executable(monte_carlo test/monte_carlo_test.cpp)
#target_link_libraries(monte_carlo ${SRC_LIST})
