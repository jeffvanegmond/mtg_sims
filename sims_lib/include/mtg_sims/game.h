#pragma once
#include "card.h"
#include "deck.h"
#include "zone.h"

#include <map>


namespace MTGSims {

class Game {
private:
	Deck deck_;
	size_t turn_;

public:
	Game(const Deck& deck);
	void reset(size_t cards = 7);

	// Game actions
	void mulligan();
	Card nextTurn();
	Card drawCard();

	size_t getTurn() const;

	ZoneClass library;
	ZoneClass hand;
	ZoneClass graveyard;
	ZoneClass exile;
	ZoneClass battlefield;
};

}
