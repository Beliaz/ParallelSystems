cmake_minimum_required(VERSION 3.5.2)
project(Exercise4 LANGUAGES CXX)


if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall")
endif()

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

#add_subdirectory(${PROJECT_SOURCE_DIR}/bench/)

aux_source_directory(. SRC_LIST)

#add_executable(n_queens test/n_queens_test.cpp)
#target_link_libraries(n_queens ${SRC_LIST})

add_executable(merge_sort test/merge_sort_test.cpp)
target_link_libraries(merge_sort ${SRC_LIST})

#add_executable(monte_carlo test/monte_carlo_test.cpp)
#target_link_libraries(monte_carlo ${SRC_LIST})
