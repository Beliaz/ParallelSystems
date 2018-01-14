cmake_minimum_required(VERSION 2.8.1)
project(real C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB real_src
    "*.h"
    "*.c"
)

link_libraries(m)

add_executable(real ${real_src})

set(CMAKE_CXX_FLAGS_FLAGS "${CMAKE_CXX_FLAGS} -g3 -std=c99 -restrict")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -march=native -fpermissive")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -opt-report -parallel -ipo")

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

include_directories(${PROJECT_SOURCE_DIR})
