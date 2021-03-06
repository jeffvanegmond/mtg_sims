#include "deck.h"
#include "card.h"
#include "zone.h"

#include <string>
#include <ostream>

namespace MTGSims {

Deck::Deck(std::string name) : name_(name) {}

void Deck::addCard(const Card& card, size_t count) {
	if(decklist_.find(card) == decklist_.end()) {
		decklist_[card] = 0;
	}
	decklist_[card] += count;
}

Card Deck::addCard(const std::string& name, size_t count) {
	Card card{name};
	addCard(card, count);
	return card;
}

void Deck::setCardCount(const Card& card, size_t count) {
	decklist_[card] = count;
}

CardList Deck::getDecklist() {
	return decklist_;
}

std::ostream& operator<<(std::ostream& out, const Deck& deck) {
	out << "Decklist for deck: " << deck.name_ << "\n";
	size_t total_cards = 0;
	for(auto entry : deck.decklist_) {
		out << entry.first << ": " << entry.second << "\n";
		total_cards += entry.second;
	}
	return out << "Total cards: " << total_cards << "\n";
}

}
