#include "mtg_sims/mtg_sims.h"

#include <iostream>

using namespace MTGSims;

class DrawSingleton {
public:

	struct Result{
		int turns = 0;
	};

	DrawSingleton(Card target_card) : target(target_card) {}
	DrawSingleton(DrawSingleton& other) : target(other.target) {}

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

DrawSingleton::Result& operator+=(DrawSingleton::Result& lhs, const DrawSingleton::Result& rhs) {
	lhs.turns += rhs.turns;
	return lhs;
}

int main(int argc, char** argv) {
	Deck deck{"Just a deck"};
	Card target = deck.addCard("Find this one");
	deck.addCard("Some random other card", 59);

	DrawSingleton draw_singleton{target};

	size_t num_sims = 1000000;
	Simulation sim{num_sims, deck};
	DrawSingleton::Result res = sim.simulate<DrawSingleton, DrawSingleton::Result>(draw_singleton);

	double average_turn = double(res.turns) / double(num_sims);
	std::cout << "Average number of turns to draw your one off: " << average_turn << "\n";

	std::cout << "Running the same simulation in parallel..." << std::endl;
	res = sim.simulateParallel<DrawSingleton, DrawSingleton::Result>(draw_singleton);
	average_turn = double(res.turns) / double(num_sims);
	std::cout << "Average number of turns to draw your one off: " << average_turn << "\n";

	return 0;
}
