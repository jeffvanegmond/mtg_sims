#pragma once
#include "game.h"
#include "deck.h"
#include "card.h"
#include "zone.h"
#include "timer.h"

#include <iostream>
#include <omp.h>

namespace MTGSims {

class Simulation {
private:
	size_t iterations_;
	Deck deck_;

public:
	Simulation(size_t iterations, Deck deck);

	template<typename T, typename R>
	R simulate(T& behavior, bool show_deck = true) {
		std::cout << "Simulation starting with " << iterations_ << " iterations.\n";
		if(show_deck) {
			std::cout << deck_;
		}
		Timer timer;
		Game game{deck_};
		R end_result;
		for(size_t i = 0; i < iterations_; ++i) {
			game.reset();
			end_result += behavior.execute(game);
		}
		std::cout << "Simulation is done and took " << timer.elapsed() << " seconds." << std::endl;
		return end_result;
	}

	template<typename T, typename R>
	R simulateParallel(T& behavior, bool show_deck = true) {
		#if defined(_OPENMP)
		std::cout << "Parallel simulation starting with " << iterations_ << " iterations.\n";
		std::cout << "Number of threads available: " << omp_get_max_threads() << "\n";
		// Setup omp, if available
		omp_set_num_threads(omp_get_max_threads());
		#else
		std::cout << "No parallelization tools available, running regular simulation instead." << std::endl;
		return simulate<T,R>(behavior, show_deck);
		#endif
		if(show_deck) {
			std::cout << deck_;
		}

		Timer timer;
		Game game{deck_};
		R end_result;
		#pragma omp parallel for firstprivate(game)
		for(size_t i = 0; i < iterations_; ++i) {
			game.reset();
			#pragma omp critical
			end_result += behavior.execute(game);
		}
		std::cout << "Simulation is done and took " << timer.elapsed() << " seconds." << std::endl;
		return end_result;
	}
};

}
