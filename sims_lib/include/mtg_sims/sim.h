#pragma once
#include "game.h"
#include "deck.h"
#include "card.h"
#include "zone.h"
#include "timer.h"

#include <iostream>

namespace MTGSims {

class Simulation {
private:
	size_t iterations_;
	Deck deck_;

public:
	Simulation(size_t iterations, Deck deck);

	template<typename T>
	void simulate(T& behavior) {
		std::cout << "Simulation starting with " << iterations_ << " iterations.\n";
		std::cout << "Using the following deck.\n" << deck_;
		Timer timer;
		#pragma omp parallel for
		for(size_t i = 0; i < iterations_; ++i) {
			Game game{deck_};
			behavior.execute(game);
		}
		std::cout << "Simulation is done and took " << timer.elapsed() << " seconds.\n";
	}

};

}
