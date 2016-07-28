cmake_minimum_required(VERSION 2.8.11)
project(mtg_sims)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_subdirectory(sims_lib)
add_subdirectory(sims)
