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
					if(game.library.size() > 0) {
						Card revealed_card = game.library.takeTopCard();
						result.damage += cmcs[revealed_card];
					}
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

void output(const Gearhulk::Result& res, size_t num_sims) {
	std::cout << "On average, this deck deals " << ((double) res.damage / (double) num_sims) << " damage with a Combustible Gearhulk" << std::endl;
	int cumulative = 0.;
	double cumulative_percentage = 0.;
	int count = 0;
	double percentage = 0.;
	for(size_t i = 0; i < res.distribution.size(); ++i) {
		count = res.distribution.at(i);
		percentage = (double)res.distribution.at(i) / (double) num_sims * 100.0;
		cumulative += count;
		cumulative_percentage += percentage;
		std::cout << i << " damage:\t" << count << "\t(" << percentage << "%)\tcumulative: " << cumulative << "\t(" << cumulative_percentage << "%)" << std::endl;
	}
}

int main(int argc, char** argv) {
	// Decklist 1:
	// http://www.bazaarofmagic.nl/deck/rb-combustible-gearhulk-di-27529.html

	Deck deck{"Standard Rb Combustible Gearhulk"};
	Card land       = deck.addCard("Land", 25);
	Card gearhulk   = deck.addCard("Combustible Gearhulk", 4);
	Card one_drop   = deck.addCard("1-drop", 0);
	Card two_drop   = deck.addCard("2-drop", 12);
	Card three_drop = deck.addCard("3-drop", 10);
	Card four_drop  = deck.addCard("4-drop", 3);
	Card five_drop  = deck.addCard("5-drop", 3);
	Card six_drop   = deck.addCard("6-drop", 0);
	Card eight_drop = deck.addCard("8-drop", 3);
	Card ten_drop      = deck.addCard("10-drop", 0);
	Card eleven_drop   = deck.addCard("11-drop", 0);
	Card thirteen_drop = deck.addCard("13-drop", 0);

	std::map<Card, int> cmcs;
	cmcs[land] = 0;
	cmcs[gearhulk] = 6;
	cmcs[one_drop] = 1;
	cmcs[two_drop] = 2;
	cmcs[three_drop] = 3;
	cmcs[four_drop] = 4;
	cmcs[five_drop] = 5;
	cmcs[six_drop] = 6;
	cmcs[eight_drop] = 8;
	cmcs[ten_drop] = 10;
	cmcs[eleven_drop] = 11;
	cmcs[thirteen_drop] = 13;

	size_t num_sims = 1000000;
	Gearhulk gearhulk_sim{land, gearhulk, cmcs};
	Simulation sim{num_sims, deck};
	Gearhulk::Result res = sim.simulateParallel<Gearhulk>(gearhulk_sim);
	output(res, num_sims);

	// Decklist 2:
	// http://www.bazaarofmagic.nl/deck/eldrazi-gearhulk-di-27527.html

	Deck deck2{"Standard Eldrazi Combustible Gearhulk"};
	land       = deck2.addCard("Land", 25);
	gearhulk   = deck2.addCard("Combustible Gearhulk", 4);
	one_drop   = deck2.addCard("1-drop", 4);
	two_drop   = deck2.addCard("2-drop", 10);
	three_drop = deck2.addCard("3-drop", 7);
	four_drop  = deck2.addCard("4-drop", 6);
	five_drop  = deck2.addCard("5-drop", 4);

	Gearhulk gearhulk_sim2{land, gearhulk, cmcs};
	Simulation sim2{num_sims, deck2};
	res = sim2.simulateParallel<Gearhulk>(gearhulk_sim2);
	output(res, num_sims);

	// Decklist 3:
	// http://www.channelfireball.com/articles/return-of-the-titans-combustible-gearhulk/
	// This is what I call "going deep". I added 2 cards because the list was incomplete!

	Deck deck3{"Standard UR Combustible Gearhulk"};
	land          = deck3.addCard("Land", 24);
	gearhulk      = deck3.addCard("Combustible Gearhulk", 3);
	two_drop      = deck3.addCard("2-drop", 10);
	three_drop    = deck3.addCard("3-drop", 10);
	four_drop     = deck3.addCard("4-drop", 4);
	eight_drop    = deck3.addCard("8-drop", 2);
	ten_drop      = deck3.addCard("10-drop", 2);
	eleven_drop   = deck3.addCard("11-drop", 4);
	thirteen_drop = deck3.addCard("13-drop", 1);

	Gearhulk gearhulk_sim3{land, gearhulk, cmcs};
	Simulation sim3{num_sims, deck3};
	res = sim3.simulateParallel<Gearhulk>(gearhulk_sim2);
	output(res, num_sims);

	std::cout << "cmcs[ten_drop] = " << cmcs[ten_drop] << std::endl;
	std::cout << "cmcs[eleven_drop] = " << cmcs[eleven_drop] << std::endl;
	std::cout << "cmcs[thirteen_drop] = " << cmcs[thirteen_drop] << std::endl;

	return 0;
}
