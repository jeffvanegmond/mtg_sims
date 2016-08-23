#include "mtg_sims/mtg_sims.h"

#include <iostream>
#include <math.h>

using namespace MTGSims;

class Burn {
public:

	struct Result{
		int turns = 0;

		// Vars used for variance calculations
		int samples = 0;
		double mean = 0;
		double M2 = 0;
		double variance = 0;
	};

	Burn(Card mountain, Card fetch, Card guide, Card bolt) : mountain(mountain), fetch(fetch), guide(guide), bolt(bolt) {}
	Burn(Burn& other) : mountain(other.mountain), fetch(other.fetch), guide(other.guide), bolt(other.bolt) {}

	Card mountain;
	Card fetch;
	Card guide;
	Card bolt;

	inline int landsInHand(const Game& game) {
		return game.hand.count(mountain) + game.hand.count(fetch);
	}

	Result execute(Game& game) {
		int lands = landsInHand(game);
//		std::cout << "My hand has " << game.hand.size() << " cards:" << std::endl;
//		std::cout << "Mountain: " << game.hand.count(mountain) << std::endl;
//		std::cout << "Fetch: " << game.hand.count(fetch) << std::endl;
//		std::cout << "Guide: " << game.hand.count(guide) << std::endl;
//		std::cout << "Bolt: " << game.hand.count(bolt) << std::endl;
		if(lands < 1 || lands > 3) {
			game.mulligan();
			lands = landsInHand(game);
			if(lands < 1 || lands > 2) {
				game.mulligan();
				lands = landsInHand(game);
				if(lands < 1 || lands > 3) {
					game.mulligan();
				}
			}
		}
//		std::cout << "I mulliganed to " << game.hand.size() << " cards.\n";

		int life = 20;
		int mana = 0;
		int unused_mana = 0;
		int damage_available = 0;
		int bolts = 0;
		int guides = 0;
		int castable_bolts = 0;
		int castable_guides = 0;
		while(life > 0) {
			// Play a fetchland if able
			if(game.hand.count(fetch) > 0 && game.library.count(mountain) > 0) {
				game.hand.takeFirst(fetch);
				game.graveyard.putCardOnTop(fetch);
				game.library.takeFirst(mountain);
				game.library.randomize();
				game.battlefield.putCardOnTop(mountain);
//				std::cout << "Played a fetchland\n";
			} else if(game.hand.count(mountain) > 0) {
				game.hand.takeFirst(mountain);
				game.battlefield.putCardOnTop(mountain);
//				std::cout << "Played a mountain\n";
			}

			mana = game.battlefield.count(mountain);
			bolts = game.hand.count(bolt);
			guides = game.hand.count(guide);

//			std::cout << "I have " << mana << " mana available. I have " << bolts << " bolts and " << guides << " guides in hand.\n";

			damage_available = game.battlefield.count(guide) * 2;
			castable_bolts = std::min<int>(mana, bolts);
			damage_available += castable_bolts * 3;
			if(castable_bolts < mana) {
				unused_mana = mana - castable_bolts;
				castable_guides = std::min<int>(unused_mana, guides);
				damage_available += castable_guides * 2;
			}
			if(damage_available >= life)
				break; // We have lethal

			// Since we don't have lethal this turn, play as many guides as possible, then spend the remainder of the mana on bolts
			castable_guides = std::min<int>(mana, guides);
			for(int i = 0; i < castable_guides; ++i) {
				game.hand.takeFirst(guide);
				game.battlefield.putCardOnTop(guide);
				--mana;
			}
			// Attack
			life -= 2 * game.battlefield.count(guide);

			castable_bolts = std::min<int>(mana, bolts);
			for(int i = 0; i < castable_bolts; ++i) {
				game.hand.takeFirst(bolt);
				game.graveyard.putCardOnTop(bolt);
				--mana;
				life -= 3;
			}
//			std::cout << "Ended turn " << game.getTurn() << " with my opponent on " << life << " life." << std::endl;
			game.nextTurn();
		}

		Result result;
		result.turns = game.getTurn();
		return result;
	}
};

Burn::Result& operator+=(Burn::Result& lhs, const Burn::Result& rhs) {
	lhs.turns += rhs.turns;

	// Rolling variance calculation, using the online algorithm of https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm
	++(lhs.samples);
	double delta = rhs.turns - lhs.mean;
	lhs.mean += delta / lhs.samples;
	lhs.M2 += delta * (rhs.turns - lhs.mean);
	if(lhs.samples > 1)
		lhs.variance = lhs.M2 / (lhs.samples -1);

	return lhs;
}

int main(int argc, char** argv) {
	Deck deck{"Modern burn deck"};
	Card fetch = deck.addCard("Fetchland", 12);
	Card mountain = deck.addCard("Mountain", 8);
	Card guide = deck.addCard("Goblin Guide", 16);
	Card bolt = deck.addCard("Lightning Bolt", 24);

	size_t num_sims = 1000000;
	int total_land = 20;
	int min_mountains = 1;
	std::vector<Burn::Result> results;
	for(int mountains = min_mountains; mountains <= total_land; ++mountains) {
		deck.setCardCount(mountain, mountains);
		deck.setCardCount(fetch, total_land - mountains);
		Burn burn{mountain, fetch, guide, bolt};
		Simulation sim{num_sims, deck};
		Burn::Result res = sim.simulateParallel<Burn, Burn::Result>(burn);
		results.push_back(res);

		double average_turn = double(res.turns) / double(num_sims);
		std::cout << "Average number of turns to kill your opponent: " << average_turn << "\n";
	}

	std::cout << "Overview:" << std::endl;
	int mountains = min_mountains;
	for(Burn::Result& res : results) {
		std::cout << "M: " << mountains << "\tF: " << total_land - mountains << "\tTurn: " << double(res.turns)/double(num_sims)
			<< "\tstd: " << std::sqrt(res.variance) << "\tn: " << res.samples << std::endl;
		++mountains;
	}
	return 0;
}
