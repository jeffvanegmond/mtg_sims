#include "game.h"
#include "card.h"
#include "zone.h"
#include "deck.h"

namespace MTGSims {

const ZoneClass& Game::getZone(Zone zone) const {
	switch(zone) {
		case Library: return library_;
		case Hand: return hand_;
		case Graveyard: return graveyard_;
		case Exile: return exile_;
	}
	return library_;
}

ZoneClass& Game::getZone(Zone zone) {
	switch(zone) {
		case Library: return library_;
		case Hand: return hand_;
		case Graveyard: return graveyard_;
		case Exile: return exile_;
	}
	return library_;
}


Game::Game(const Deck& deck) :
  deck_(deck) {
	reset();
}

Game::Game(Game& other_game) :
  deck_(other_game.deck_) {
	reset();
}

void Game::reset(size_t cards) {
	library_.initialize(deck_.getDecklist());
	library_.randomize();

	hand_.clear();
	graveyard_.clear();
	exile_.clear();

	for(size_t i = 0; i < cards; ++i) {
		drawCard();
	}

	turn_ = 1;
}

void Game::mulligan() {
	reset(hand_.size()-1);
}

Card Game::nextTurn() {
	++turn_;
	return drawCard();
}

Card Game::drawCard() {
	Card drawn = library_.takeTopCard();
	hand_.putCardOnTop(drawn);
	return drawn;
}

void Game::moveCard(Zone from, Zone to, const Card& card) {
	getZone(from).takeFirst(card);
	getZone(to).putCardOnTop(card);
}

void Game::shuffle(Zone zone) {
	getZone(zone).randomize();
}

bool Game::zoneContains(Zone zone, const Card& card) const {
	return getZone(zone).contains(card);
}

size_t Game::zoneSize(Zone zone) const{
	return getZone(zone).size();
}

size_t Game::zoneCount(Zone zone, const Card& card) const {
	return getZone(zone).count(card);
}

size_t Game::getTurn() const {
	return turn_;
}

}
