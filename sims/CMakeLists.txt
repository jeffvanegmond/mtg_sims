include_directories(${CMAKE_SOURCE_DIR}/sims_lib/include)
add_executable(draw_singleton src/draw_singleton.cpp)
target_link_libraries(draw_singleton mtg_sims)
