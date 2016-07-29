#include "mtg_sims/mtg_sims.h"

#include <iostream>

using namespace MTGSims;

class DrawSingleton {
public:
	DrawSingleton(Card target_card) : target(target_card) {}
	Card target;
	int cumulative_turns = 0;
	void execute(Game& game) {
		while(!game.zoneContains(Zone::Hand, target)) {
			game.nextTurn();
		}
		cumulative_turns += game.getTurn();
	}
};

int main(int argc, char** argv) {
	Deck deck{"Just a deck"};
	Card target = deck.addCard("Find this one");
	deck.addCard("Some random other card", 59);

	DrawSingleton draw_singleton{target};

	size_t num_sims = 100000;
	Simulation sim{num_sims, deck};
	sim.simulate<DrawSingleton>(draw_singleton);

	double average_turn = double(draw_singleton.cumulative_turns) / double(num_sims);
	std::cout << "Average number of turns to draw your one off: " << average_turn << "\n";

	return 0;
}
