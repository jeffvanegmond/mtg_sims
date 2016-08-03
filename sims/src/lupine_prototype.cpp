#include "mtg_sims/mtg_sims.h"

#include <iostream>
#include <vector>

using namespace MTGSims;

class LupinePrototype {
public:
	static constexpr bool verbose = false;
	static constexpr int max_turns = 5;

	struct Result{
		std::vector<int> lupine_on_turn;
		std::vector<int> hellbent_on_turn;
		std::vector<int> active_on_turn;
		Result() {
			lupine_on_turn.assign(max_turns, 0);
			hellbent_on_turn.assign(max_turns, 0);
			active_on_turn.assign(max_turns, 0);
		}
	};

	Card darksteel_citadel;
	Card red_source;
	Card nexus;
	Card mox_opal;
	Card springleaf_drum;
	Card zero_drop;
	Card one_drop;
	Card two_drop;
	Card lupine_prototype;
	Card frogmite;
	Card myr_enforcer;

	LupinePrototype(Card cards[]) {
		int i = 0;
		darksteel_citadel = cards[i++];
		red_source        = cards[i++];
		nexus             = cards[i++];
		mox_opal          = cards[i++];
		springleaf_drum   = cards[i++];
		zero_drop         = cards[i++];
		one_drop          = cards[i++];
		two_drop          = cards[i++];
		lupine_prototype  = cards[i++];
		frogmite          = cards[i++];
		myr_enforcer      = cards[i++];
	}

	inline int landsInPlay(const Game& game) {
		return game.battlefield.count(darksteel_citadel) +
		       game.battlefield.count(red_source) +
		       game.battlefield.count(nexus);
	}

	inline int artifactsInPlay(const Game& game) {
		return game.battlefield.count(darksteel_citadel) +
		       game.battlefield.count(mox_opal) +
		       game.battlefield.count(springleaf_drum) +
		       game.battlefield.count(zero_drop) +
		       game.battlefield.count(one_drop) +
		       game.battlefield.count(two_drop) +
		       game.battlefield.count(lupine_prototype) +
		       game.battlefield.count(frogmite) +
		       game.battlefield.count(myr_enforcer);
	}

	inline int creaturesInPlay(const Game& game) {
		       game.battlefield.count(zero_drop) +
		       game.battlefield.count(one_drop) +
		       game.battlefield.count(two_drop) +
		       game.battlefield.count(lupine_prototype) +
		       game.battlefield.count(frogmite) +
		       game.battlefield.count(myr_enforcer);
	}

	inline bool metalcraft(const Game& game) {
		return artifactsInPlay(game) >= 3;
	}

	inline int affinityCost(const Game& game, int cost) {
		return std::max<int>(0, cost - artifactsInPlay(game));
	}

	Result execute(Game& game) {
		// Execute mulligans
		int mana_sources;
		while(game.hand.size() > 1) {
			mana_sources = game.hand.count(darksteel_citadel) +
			               game.hand.count(red_source) +
			               game.hand.count(nexus) +
			               game.hand.count(mox_opal) +
			               game.hand.count(springleaf_drum);
			if(!(mana_sources < 2 || mana_sources > 5))
				break;
			game.mulligan();
		}
		if(verbose)
			std::cout << "\n\nI mulliganed this game to " << game.hand.size() << " cards.\n";

		// Perform the game
		Result result;
		int mana, tapped_drums, tappable_drums;
		bool action_taken, tapped_opal;
		while(game.getTurn() <= max_turns) {
			tapped_drums = 0;
			tapped_opal = false;
			mana = 0;
			if(verbose)
				std::cout << "It is now turn " << game.getTurn() << "\n";

			// 1. Play a land
			if(game.hand.contains(darksteel_citadel)) {
				game.hand.takeFirst(darksteel_citadel);
				game.battlefield.putCardOnTop(darksteel_citadel);
				if(verbose)
					std::cout << "Played " << darksteel_citadel << "\n";
			}
			else if(game.hand.contains(red_source)) {
				game.hand.takeFirst(red_source);
				game.battlefield.putCardOnTop(red_source);
				if(verbose)
					std::cout << "Played " << red_source << "\n";
			} else if(game.hand.contains(nexus)) {
				game.hand.takeFirst(nexus);
				game.battlefield.putCardOnTop(nexus);
				if(verbose)
					std::cout << "Played " << nexus << "\n";
			}

			// 2.a Play a mox if we don't have one in play
			if(!game.battlefield.contains(mox_opal) && game.hand.contains(mox_opal)) {
				game.hand.takeFirst(mox_opal);
				game.battlefield.putCardOnTop(mox_opal);
				if(verbose)
					std::cout << "Played " << mox_opal << "\n";
			}

			// 2.b Play all 0-drop creatures
			while(game.hand.contains(zero_drop)) {
				game.hand.takeFirst(zero_drop);
				game.battlefield.putCardOnTop(zero_drop);
				if(verbose)
					std::cout << "Played " << zero_drop << "\n";
			}

			// 3. Tap all lands for mana
			mana = landsInPlay(game);
			if(verbose)
				std::cout << "Made " << mana << " mana with my lands.\n";

			action_taken = true;
			while(action_taken) {
				if(verbose)
					std::cout << "I currently have " << mana << " mana available.\n";
				action_taken = false;
				// 4. Try tapping moxen and drums for mana
				if(metalcraft(game) && game.battlefield.contains(mox_opal) && !tapped_opal) {
					++mana;
					if(verbose)
						std::cout << "Made mana (" << mana << ") with my " << mox_opal << " that was in play.\n";
					tapped_opal = true;
					// Naively play all moxen now and make mana with them
					while(game.hand.contains(mox_opal)) {
						++mana;
						game.hand.takeFirst(mox_opal);
						game.graveyard.putCardOnTop(mox_opal);
						if(verbose)
							std::cout << "Played " << mox_opal << " and made a mana (" << mana << ")\n";
					}
					action_taken = true;
				}
				tappable_drums = std::max<int>(0, game.battlefield.count(springleaf_drum) - tapped_drums - creaturesInPlay(game));
				if(tapped_drums > 0) {
					mana += tappable_drums;
					tapped_drums += tappable_drums;
					if(verbose)
						std::cout << "Tapped " << tappable_drums << " for mana (" << mana << ")\n";
					action_taken = true;
				}

				// Attempt to cast drums
				while(game.hand.contains(springleaf_drum) && mana > 0) {
					game.hand.takeFirst(springleaf_drum);
					game.battlefield.putCardOnTop(springleaf_drum);
					--mana;
					if(verbose)
						std::cout << "Played " << springleaf_drum << "\n";
					action_taken = true;
				}

				// Attempt to cast Lupine
				while(game.hand.contains(lupine_prototype) && mana > 1) {
					game.hand.takeFirst(lupine_prototype);
					game.battlefield.putCardOnTop(lupine_prototype);
					if(verbose)
						std::cout << "Played " << lupine_prototype << "\n";
					mana -= 2;
					action_taken = true;
				}

				// 6. Attempt to cast other 2-drops
				while(game.hand.contains(two_drop) && mana > 1) {
					game.hand.takeFirst(two_drop);
					game.battlefield.putCardOnTop(two_drop);
					if(verbose)
						std::cout << "Played " << two_drop << "\n";
					mana -= 2;
					action_taken = true;
				}

				// Attempt to cast 1-drops
				while(game.hand.contains(one_drop) && mana > 0) {
					game.hand.takeFirst(one_drop);
					game.battlefield.putCardOnTop(one_drop);
					if(verbose)
						std::cout << "Played " << one_drop << "\n";
					--mana;
					action_taken = true;
				}

				// 7. Dump affinity cards in play
				while(game.hand.contains(frogmite) && mana > affinityCost(game, 4)) {
					mana -= affinityCost(game, 4);
					game.hand.takeFirst(frogmite);
					game.battlefield.putCardOnTop(frogmite);
					if(verbose)
						std::cout << "Played " << frogmite << "\n";
					action_taken = true;
				}
				while(game.hand.contains(myr_enforcer) && mana > affinityCost(game, 7)) {
					mana -= affinityCost(game, 4);
					game.hand.takeFirst(myr_enforcer);
					game.battlefield.putCardOnTop(myr_enforcer);
					if(verbose)
						std::cout << "Played " << myr_enforcer << "\n";
					action_taken = true;
				}
			}

			// Check if we have an active* lupine
			// *active defined as in play and being hellbent.
			if(verbose) {
				std::cout << "I " << (game.battlefield.contains(lupine_prototype) ? "DO" : "DON'T") << " have a " << lupine_prototype << " in play.\n";
				std::cout << "I have " << game.hand.size() << " cards in hand.\n";
			}
			bool has_lupine = game.battlefield.contains(lupine_prototype);
			bool hellbent = game.hand.size() == 0;
			bool active_lupine = has_lupine && hellbent;
			int idx = game.getTurn() - 1;
			if(has_lupine)
				result.lupine_on_turn[idx] = 1;
			if(hellbent)
				result.hellbent_on_turn[idx] = 1;
			if(active_lupine)
				result.active_on_turn[idx] = 1;
			if(verbose)
				std::cout << "Finishing my turn." << std::endl;
			game.nextTurn();
		}
		return result;
	}
};

LupinePrototype::Result& operator+=(LupinePrototype::Result& lhs, const LupinePrototype::Result& rhs) {
	for(size_t i = 0; i < LupinePrototype::max_turns; ++i) {
		lhs.lupine_on_turn[i] += rhs.lupine_on_turn[i];
		lhs.hellbent_on_turn[i] += rhs.hellbent_on_turn[i];
		lhs.active_on_turn[i] += rhs.active_on_turn[i];
	}
	return lhs;
}

int main(int argc, char** argv) {
	Deck deck{"Lupine Prototype Affinity"};
	Card darksteel_citadel = deck.addCard("Darksteel Citadel", 4);
	Card red_source        = deck.addCard("Red Source", 5); // Glimmervoid or mountain
	Card nexus             = deck.addCard("Nexus", 8); // Inkmoth or blinkmoth
	Card mox_opal          = deck.addCard("Mox Opal", 4);
	Card springleaf_drum   = deck.addCard("Springleaf Drum", 4);
	Card zero_drop         = deck.addCard("Zero Drop Dude", 7); // Memnite or Ornithopter
	Card one_drop          = deck.addCard("One Drop Dude", 8);
	Card two_drop          = deck.addCard("Two Drop Dude", 8);
	Card lupine_prototype  = deck.addCard("Lupine Prototype", 4);
	Card frogmite          = deck.addCard("Frogmite", 4);
	Card myr_enforcer      = deck.addCard("Myr Enforcer", 4);
	Card cards[] = {darksteel_citadel, red_source, nexus, mox_opal, springleaf_drum, zero_drop, one_drop, two_drop, lupine_prototype, frogmite, myr_enforcer};

	LupinePrototype lupine_test{cards};

	size_t num_sims = 100000;
	Simulation sim{num_sims, deck};
	LupinePrototype::Result res = sim.simulateParallel<LupinePrototype>(lupine_test);
	//LupinePrototype::Result res = sim.simulate<LupinePrototype>(lupine_test);
	int turn;
	std::cout << "==== Results by turn ====\n";
	for(int i = 0; i < LupinePrototype::max_turns; ++i) {
		turn = i+1;
		std::cout << "-- Turn: " << turn << "\n";
		std::cout << "  Lupine in Play on turn " << turn << ":\t" << res.lupine_on_turn[i] << "\t(" << double(res.lupine_on_turn[i]) / num_sims * 100.0 << "%)\n";
		std::cout << "  Hellbent on turn " << turn << ":\t\t" << res.hellbent_on_turn[i] << "\t(" << double(res.hellbent_on_turn[i]) / num_sims * 100.0 << "%)\n";
		std::cout << "  Active Lupine on turn " << turn << ":\t" << res.active_on_turn[i] << "\t(" << double(res.active_on_turn[i]) / num_sims * 100.0 << "%)\n";
	}

	std::cout << "==== Results by type ====\n";
	for(int i = 0; i < LupinePrototype::max_turns; ++i) {
		turn = i+1;
		std::cout << "  Lupine in Play on turn " << turn << ":\t" << res.lupine_on_turn[i] << "\t(" << double(res.lupine_on_turn[i]) / num_sims * 100.0 << "%)\n";
	}
	for(int i = 0; i < LupinePrototype::max_turns; ++i) {
		turn = i+1;
		std::cout << "  Hellbent on turn " << turn << ":\t\t" << res.hellbent_on_turn[i] << "\t(" << double(res.hellbent_on_turn[i]) / num_sims * 100.0 << "%)\n";
	}
	for(int i = 0; i < LupinePrototype::max_turns; ++i) {
		turn = i+1;
		std::cout << "  Active Lupine on turn " << turn << ":\t" << res.active_on_turn[i] << "\t(" << double(res.active_on_turn[i]) / num_sims * 100.0 << "%)\n";
	}

	return 0;
}
