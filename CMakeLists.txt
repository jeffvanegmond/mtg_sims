cmake_minimum_required(VERSION 2.8.11)
project(mtg_sims)

SET(CMAKE_CXX_FLAGS "-std=c++11")
SET(CMAKE_BUILD_TYPE RELEASE)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(sims_lib)
add_subdirectory(sims)
