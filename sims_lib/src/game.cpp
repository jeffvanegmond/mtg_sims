#include "game.h"
#include "card.h"
#include "zone.h"
#include "deck.h"

namespace MTGSims {

Game::Game(const Deck& deck) :
  deck_(deck), library("library"), hand("hand"), graveyard("graveyard"), exile("exile"), battlefield("battlefield") {
	reset();
}

//Game::Game(Game& other_game) :
//  deck_(other_game.deck_){
//	reset();
//}

void Game::reset(size_t cards) {
	library.initialize(deck_.getDecklist());
	library.randomize();

	hand.clear();
	graveyard.clear();
	exile.clear();

	for(size_t i = 0; i < cards; ++i) {
		drawCard();
	}

	turn_ = 1;
}

void Game::mulligan() {
	reset(hand.size()-1);
}

Card Game::nextTurn() {
	++turn_;
	return drawCard();
}

Card Game::drawCard() {
	Card drawn = library.takeTopCard();
	hand.putCardOnTop(drawn);
	return drawn;
}

size_t Game::getTurn() const {
	return turn_;
}

}
