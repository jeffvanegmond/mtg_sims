#include "mtg_sims/mtg_sims.h"

#include <iostream>
#include <math.h>

using namespace MTGSims;

class Gearhulk {
public:

	struct Result{
		int damage = 0;

		// Vars used for variance calculations
		std::vector<int> distribution;
	};

	Gearhulk(Card land, Card gearhulk, std::map<Card, int> cmcs)  :
	    land(land), gearhulk(gearhulk), cmcs(cmcs) {}
	Gearhulk(Gearhulk& other) :
	    land(other.land), gearhulk(other.gearhulk), cmcs(other.cmcs) {}

	Card land;
	Card gearhulk;
	std::map<Card, int> cmcs;

	Result execute(Game& game) {
		// Determine mulligan 
		int lands = game.hand.count(land);
		if(lands < 2 || lands > 5) { // Mulligan hands with less than 2 or more than 5 lands at 7 cards
			game.mulligan();
			lands = game.hand.count(land);
			if(lands < 2 || lands > 5) { // Mulligan hands with less than 2 or more than 5 lands at 6 cards
				game.mulligan();
				lands = game.hand.count(land);
				if(lands < 1 || lands > 4) { // Mulligan hands with less than 1 or more than 4 lands at 5 cards
					game.mulligan();
					lands = game.hand.count(land);
					if(lands < 1) { // Mulligan hands with less than 1 or more than 5 lands at 4 cards
						game.mulligan();
						// Keep at 3 cards regardless of what is in hand
					}
				}
			}
		}

		Result result;
		while(true) {
			// Play a land if we can.
			// If we have 6 lands in play and a gearhulk, cast it.
			// Assume we always deal damage with the gearhulk and count the damage, store that as our result.

			if(game.hand.count(land) > 0) {
				game.hand.takeFirst(land);
				game.battlefield.putCardOnTop(land);
			}

			if(game.battlefield.count(land) < 6) {
				game.nextTurn();
				continue;
			}

			if(game.hand.count(gearhulk) > 0) {
				for(int i = 0; i < 3; ++i) {
					Card revealed_card = game.library.takeTopCard();
					result.damage += cmcs[revealed_card];
				}
				break;
			}
			game.nextTurn();
		}

		return result;
	}
};

Gearhulk::Result& operator+=(Gearhulk::Result& lhs, const Gearhulk::Result& rhs) {
	lhs.damage += rhs.damage;

	if(lhs.distribution.size() <= rhs.damage + 1) {
		lhs.distribution.resize(rhs.damage + 1, 0);
	}
	lhs.distribution.at(rhs.damage) += 1;

	return lhs;
}

int main(int argc, char** argv) {
	Deck deck{"Standard Combustible Gearhulk"};
	Card land       = deck.addCard("Land", 26);
	Card gearhulk   = deck.addCard("Combustible Gearhulk", 4);
	Card one_drop   = deck.addCard("1-drop", 4);
	Card two_drop   = deck.addCard("2-drop", 6);
	Card three_drop = deck.addCard("3-drop", 9);
	Card four_drop  = deck.addCard("4-drop", 6);
	Card five_drop  = deck.addCard("5-drop", 4);
	Card six_drop   = deck.addCard("6-drop", 1);
	std::map<Card, int> cmcs;
	cmcs[land] = 0;
	cmcs[gearhulk] = 6;
	cmcs[one_drop] = 1;
	cmcs[two_drop] = 2;
	cmcs[three_drop] = 3;
	cmcs[four_drop] = 4;
	cmcs[five_drop] = 5;
	cmcs[six_drop] = 6;

	size_t num_sims = 1000000;
	Gearhulk gearhulk_sim{land, gearhulk, cmcs};
	Simulation sim{num_sims, deck};
	Gearhulk::Result res = sim.simulateParallel<Gearhulk, Gearhulk::Result>(gearhulk_sim);

	std::cout << "On average, this deck deals " << ((double) res.damage / (double) num_sims) << " damage with a Combustible Gearhulk" << std::endl;

	for(size_t i = 0; i < res.distribution.size(); ++i) {
		std::cout << i << " damage:\t" << res.distribution.at(i) << "(" << 100.0 * (double) res.distribution.at(i) / (double) num_sims << "%)" << std::endl;
	}

	return 0;
}
