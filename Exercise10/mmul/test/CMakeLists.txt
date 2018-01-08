#~~~~~~~~~~~~~~~~

file(GLOB_RECURSE ALL_TEST_CPP *.cpp)
 
foreach(ONE_TEST_CPP ${ALL_TEST_CPP})

    get_filename_component(ONE_TEST_EXEC ${ONE_TEST_CPP} NAME_WE)

    set(TARGET_NAME ut_${ONE_TEST_EXEC})

    add_executable(${TARGET_NAME} ${ONE_TEST_CPP})
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${ONE_TEST_EXEC}) 

    add_test(NAME ${TARGET_NAME}_1 COMMAND "mpirun" -n 1 ${ONE_TEST_EXEC})
    add_test(NAME ${TARGET_NAME}_4 COMMAND "mpirun" -n 4 ${ONE_TEST_EXEC})
    add_test(NAME ${TARGET_NAME}_16 COMMAND "mpirun" -n 16 ${ONE_TEST_EXEC})
    add_test(NAME ${TARGET_NAME}_64 COMMAND "mpirun" -n 64 ${ONE_TEST_EXEC})

    find_package(MPI REQUIRED)
    if(MPI_FOUND)
        set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
        set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
        include_directories(MPI_INCLUDE_PATH)
        target_link_libraries(${TARGET_NAME} ${MPI_LIBRARIES}) 
    endif()

    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ENDIF()

	if(MSVC)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++latest /wd4141 /arch:AVX2")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /arch:AVX2 /DNDEBUG")
	else()
	    set(CMAKE_C_COMPILER mpicc)
		set(CMAKE_CXX_COMPILER mpicxx)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -march=native")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
	endif()

	set(CMAKE_BUILD_TYPE Release)

	include_directories(${PROJECT_SOURCE_DIR})

endforeach()