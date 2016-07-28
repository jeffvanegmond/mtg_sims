#pragma once
#include "card.h"

#include <map>
#include <ostream>
#include <string>

namespace MTGSims {

typedef std::map<Card, size_t> CardList;

class Deck {
private:
	// Original decklist
	std::string name_;
	CardList decklist_;

public:
	Deck(std::string name);

	// Initialization functions
	void addCard(const Card& card, size_t count = 1);
	Card addCard(const std::string& name, size_t count = 1);

	// Access function
	CardList getDecklist();

	// Utility functions
	friend std::ostream& operator<<(std::ostream& out, const Deck& deck);
};

}
