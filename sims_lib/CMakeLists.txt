SET(LIB_src src/card.cpp src/game.cpp src/deck.cpp src/zone.cpp)

include_directories(include/mtg_sims)
add_library(mtg_sims ${LIB_src})
