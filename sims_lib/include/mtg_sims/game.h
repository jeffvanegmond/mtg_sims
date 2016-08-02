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

	ZoneClass library_;
	ZoneClass hand_;
	ZoneClass graveyard_;
	ZoneClass exile_;

	const ZoneClass& getZone(Zone zone) const;
	ZoneClass& getZone(Zone zone);

public:
	Game(const Deck& deck);
	Game(Game& other_game);
	void reset(size_t cards = 7);

	// Game actions
	void mulligan();
	Card nextTurn();
	Card drawCard();
	void moveCard(Zone from, Zone to, const Card& card);
	void shuffle(Zone zone = Zone::Library);

	// Game status functions
	bool zoneContains(Zone zone, const Card& card) const;
	size_t zoneSize(Zone zone) const;
	size_t zoneCount(Zone zone, const Card& card) const;
	size_t getTurn() const;

};

}
