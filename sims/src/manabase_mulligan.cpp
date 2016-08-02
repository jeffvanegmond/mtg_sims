#include "mtg_sims/mtg_sims.h"

#include <iostream>

using namespace MTGSims;

class ManabaseMull {
public:
	ManabaseMull(Card red_source, Card other_land) : red_source(red_source), other_land(other_land) {}
	ManabaseMull(ManabaseMull& other) : red_source(other.red_source), other_land(other.other_land) {}
	Card red_source;
	Card other_land;

	struct Result {
		int keep_7 = 0;
		int keep_6 = 0;
		int keep_5 = 0;
		int keep_4 = 0;
	};

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

	Result execute(Game& game) {
		while(!keepHand(game)) {
			game.mulligan();
		}
		Result r;
		switch(game.zoneSize(Zone::Hand)) {
			case 7:
				++r.keep_7;
				break;
			case 6:
				++r.keep_6;
				break;
			case 5:
				++r.keep_5;
				break;
			case 4:
			default:
				++r.keep_4;
		}
		return r;
	}
};

ManabaseMull::Result& operator+=(ManabaseMull::Result& lhs, const ManabaseMull::Result& rhs) {
	lhs.keep_4 += rhs.keep_4;
	lhs.keep_5 += rhs.keep_5;
	lhs.keep_6 += rhs.keep_6;
	lhs.keep_7 += rhs.keep_7;
	return lhs;
}

int main(int argc, char** argv) {
	Deck deck{"Manabase checkup"};
	deck.addCard("Some random other card", 36);
	Card red_source{"Red source"};
	Card other_land{"Other land"};
	for(size_t red_sources = 10; red_sources < 18; ++red_sources) {
		deck.setCardCount(red_source, red_sources);
		deck.setCardCount(other_land, 24 - red_sources);

		ManabaseMull manabase_mull{red_source, other_land};

		size_t num_sims = 100000;
		Simulation sim{num_sims, deck};
		ManabaseMull::Result res = sim.simulateParallel<ManabaseMull, ManabaseMull::Result>(manabase_mull);

		std::cout << "Seven cards kept: " << res.keep_7 << "\t(" << double(res.keep_7) / num_sims << "%)\n";
		std::cout << "Six cards kept:   " << res.keep_6 << "\t(" << double(res.keep_6) / num_sims << "%)\n";
		std::cout << "Five cards kept:  " << res.keep_5 << "\t(" << double(res.keep_5) / num_sims << "%)\n";
		std::cout << "Four cards kept:  " << res.keep_4 << "\t(" << double(res.keep_4) / num_sims << "%)\n";
	}

	return 0;
}
