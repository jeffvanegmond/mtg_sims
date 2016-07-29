#include "sim.h"
#include "timer.h"
#include "card.h"
#include "deck.h"
#include "zone.h"
#include "game.h"

namespace MTGSims {

Simulation::Simulation(size_t iterations, Deck deck) : iterations_(iterations), deck_(deck) {}

}
