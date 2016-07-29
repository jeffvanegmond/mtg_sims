#include "mtg_sims/game.h"
#include "mtg_sims/deck.h"
#include "mtg_sims/zone.h"
#include "mtg_sims/card.h"
#include "mtg_sims/timer.h"

#include <iostream>

using namespace MTGSims;

int main(int argc, char** argv) {
	Timer timer;

	Deck deck{"Just a deck"};
	Card target_card = deck.addCard("Find this one");
	Card other_card  = deck.addCard("Some random other card", 59);

	std::cout << deck;

	Game game{deck};
	int num_sims = 1000000;
	int cumulative_turns_to_find = 0;
	for(int i = 0; i < num_sims; ++i) {
		game.reset();
		while(!game.zoneContains(Zone::Hand, target_card)) {
			game.nextTurn();
		}
		cumulative_turns_to_find += game.getTurn();
	}

	double average_turn = double(cumulative_turns_to_find) / double(num_sims);

	std::cout << "Average number of turns to draw your one off: " << average_turn << "\n";
	std::cout << "It took " << timer.elapsed() << " seconds to find this out with " << num_sims << " simulations." << std::endl;

	return 0;
}
