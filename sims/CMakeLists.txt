include_directories(${CMAKE_SOURCE_DIR}/sims_lib/include)

add_executable(draw_singleton src/draw_singleton.cpp)
target_link_libraries(draw_singleton mtg_sims)

add_executable(manabase_mulligan src/manabase_mulligan.cpp)
target_link_libraries(manabase_mulligan mtg_sims)

add_executable(lupine_prototype src/lupine_prototype.cpp)
target_link_libraries(lupine_prototype mtg_sims)

add_executable(burn_fetch src/burn_fetch.cpp)
target_link_libraries(burn_fetch mtg_sims)

add_executable(combustible_gearhulk src/combustible_gearhulk.cpp)
target_link_libraries(combustible_gearhulk mtg_sims)
