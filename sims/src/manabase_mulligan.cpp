#include "mtg_sims/mtg_sims.h"

#include <iostream>

using namespace MTGSims;

class ManabaseMull {
public:
	ManabaseMull(Card red_source, Card other_land) : red_source(red_source), other_land(other_land) {}
	Card red_source;
	Card other_land;

	int keep_7 = 0;
	int keep_6 = 0;
	int keep_5 = 0;
	int keep_4 = 0;

	bool keepHand(Game& game) {
		int num_red = game.zoneCount(Zone::Hand, red_source);
		int num_other_land = game.zoneCount(Zone::Hand, other_land);
		int num_land = num_red + num_other_land;
		switch(game.zoneSize(Zone::Hand)) {
			case 7:
			case 6:
				return num_land > 1 && num_land < 5 && num_red > 0;
			case 5:
				return num_land > 0 && num_land < 5 && num_red > 0;
			case 4:
			default:
				return true;
		}
		return true;
	}

	void execute(Game& game) {
		while(!keepHand(game)) {
			game.mulligan();
		}
		switch(game.zoneSize(Zone::Hand)) {
			case 7:
				++keep_7;
				break;
			case 6:
				++keep_6;
				break;
			case 5:
				++keep_5;
				break;
			case 4:
			default:
				++keep_4;
		}
	}
};

int main(int argc, char** argv) {
	Deck deck{"Manabase checkup"};
	Card red_source = deck.addCard("Red source", 14);
	Card other_land = deck.addCard("Other land", 10);
	deck.addCard("Some random other card", 36);

	ManabaseMull manabase_mull{red_source, other_land};

	size_t num_sims = 100000;
	Simulation sim{num_sims, deck};
	sim.simulate<ManabaseMull>(manabase_mull);

	std::cout << "Seven cards kept: " << manabase_mull.keep_7 << "\t(" << double(manabase_mull.keep_7) / num_sims << "%)\n";
	std::cout << "Six cards kept:   " << manabase_mull.keep_6 << "\t(" << double(manabase_mull.keep_6) / num_sims << "%)\n";
	std::cout << "Five cards kept:  " << manabase_mull.keep_5 << "\t(" << double(manabase_mull.keep_5) / num_sims << "%)\n";
	std::cout << "Four cards kept:  " << manabase_mull.keep_4 << "\t(" << double(manabase_mull.keep_4) / num_sims << "%)\n";
	return 0;
}
