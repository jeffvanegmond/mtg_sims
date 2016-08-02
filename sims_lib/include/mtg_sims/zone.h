#pragma once
#include "card.h"

#include <map>
#include <deque>
#include <ostream>
#include <random>
#include <string>

namespace MTGSims {

typedef std::map<Card, size_t> CardList;

class ZoneClass {
private:
	CardList zone_counts_;
	std::deque<Card> ordered_zone_;
	std::default_random_engine engine_;
	std::string name_;

	void reseedRandom();

public:
	ZoneClass(const std::string& name);

	// Initialization stuff
	void clear();
	void initialize(const CardList& initialize_with);

	// Manipulation stuff
	void randomize();
	Card takeTopCard();
	Card takeBottomCard();
	bool takeFirst(const Card& card);
	bool takeLast(const Card& card);
	void putCardOnTop(const Card& card);
	void putCardOnBottom(const Card& card);

	// Checking stuff
	size_t size() const;
	size_t count(const Card& card) const;
	bool contains(const Card& card) const;
};


}
