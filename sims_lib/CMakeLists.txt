SET(LIB_src src/card.cpp src/game.cpp src/deck.cpp src/zone.cpp src/timer.cpp src/sim.cpp)

include_directories(include/mtg_sims)
add_library(mtg_sims ${LIB_src})
