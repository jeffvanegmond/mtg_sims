#include "mtg_sims/mtg_sims.h"

#include <iostream>

using namespace MTGSims;

class LupinePrototype {
public:

	struct Result{
		int turns = 0;
	};

	LupinePrototype(Card target_card) : target(target_card) {}
	LupinePrototype(LupinePrototype& other) : target(other.target) {}

	Card target;

	Result execute(Game& game) {
		while(!game.hand.contains(target)) {
			game.nextTurn();
		}
		Result result;
		result.turns = game.getTurn();
		return result;
	}
};

LupinePrototype::Result& operator+=(LupinePrototype::Result& lhs, const LupinePrototype::Result& rhs) {
	lhs.turns += rhs.turns;
	return lhs;
}

int main(int argc, char** argv) {
	Deck deck{"Lupine Prototype Affinity"};
	Card darksteel_citadel = deck.addCard("Darksteel Citadel", 4);
	Card red_source = deck.addCard("Red Source", 5); // Glimmervoid or mountain
	Card nexus = deck.addCard("Nexus", 8); // Inkmoth or blinkmoth
	Card chrome_mox = deck.addCard("Chrome Mox", 4);
	Card springleaf_drum = deck.addCard("Springleaf Drum", 4);
	Card zero_drop = deck.addCard("Zero Drop Dude", 7); // Memnite or Ornithopter
	Card one_drop = deck.addCard("One Drop Dude", 8);
	Card two_drop = deck.addCard("Two Drop Dude", 8);
	Card lupine_prototype = deck.addCard("Lupine Prototype", 4);
	Card frogmite = deck.addCard("Frogmite", 4);
	Card myr_enforcer = deck.addCard("Myr Enforcer", 4);

	LupinePrototype draw_singleton{lupine_prototype};

	size_t num_sims = 1000000;
	Simulation sim{num_sims, deck};
	LupinePrototype::Result res = sim.simulate<LupinePrototype, LupinePrototype::Result>(draw_singleton);

	double average_turn = double(res.turns) / double(num_sims);
	std::cout << "Average number of turns to draw your one off: " << average_turn << "\n";

	std::cout << "Running the same simulation in parallel..." << std::endl;
	res = sim.simulateParallel<LupinePrototype, LupinePrototype::Result>(draw_singleton);
	average_turn = double(res.turns) / double(num_sims);
	std::cout << "Average number of turns to draw your Lupine Prototype: " << average_turn << "\n";

	return 0;
}
