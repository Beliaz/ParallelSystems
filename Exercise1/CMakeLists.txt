cmake_minimum_required(VERSION 3.6)
project(Exercise1)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /WX")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif()

set(SOURCE_FILES mmul.cpp)
add_executable(Exercise1 ${SOURCE_FILES})
