cmake_minimum_required(VERSION 3.5.2)
project(Exercise2 LANGUAGES CXX)

# cmake module directory
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
        cmake)

# This include is required if you want 
# to use CTest framework for you benchmarks with add_test()
#
include(CTest) 

enable_testing()

set(BENCHMARK_ENABLE_TESTING OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /DNDEBUG /DCONTIGUOUS_WITH_INDIRECTION")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DCONTIGUOUS_WITH_INDIRECTION")
endif()

FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

include(ConfigGBench)

add_subdirectory(${PROJECT_SOURCE_DIR}/test/)
add_subdirectory(${PROJECT_SOURCE_DIR}/bench/)

set(SOURCE_FILES main.cpp)

add_executable(Exercise2_indirection ${SOURCE_FILES})
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /DNDEBUG /DCONTIGUOUS_WITH_MULTIPLICATION")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DCONTIGUOUS_WITH_MULTIPLICATION")
endif()
add_executable(Exercise2_multiplication ${SOURCE_FILES})
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /wd4141 /WX")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /DNDEBUG /DNESTED_VECTOR")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DNESTED_VECTOR")
endif()
add_executable(Exercise2_nested ${SOURCE_FILES})
