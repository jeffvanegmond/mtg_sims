#include "zone.h"
#include "card.h"

#include <algorithm>
#include <random>
#include <chrono>

namespace MTGSims {

ZoneClass::ZoneClass(const std::string& name) : name_(name) {
	reseedRandom();
}

void ZoneClass::reseedRandom() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	engine_ = std::default_random_engine{seed};
}

void ZoneClass::clear() {
	zone_counts_.clear();
	ordered_zone_.clear();
}

void ZoneClass::initialize(const CardList& initialize_with) {
	clear();
	zone_counts_ = initialize_with;
	for(auto entry : zone_counts_) {
		for(size_t i = 0; i < entry.second; ++i) {
			ordered_zone_.push_back(entry.first);
		}
	}
}

void ZoneClass::randomize() {
	std::shuffle(std::begin(ordered_zone_), std::end(ordered_zone_), engine_);
}

Card ZoneClass::takeTopCard() {
	Card card = ordered_zone_.front();
	ordered_zone_.pop_front();
	zone_counts_[card] -= 1;
	return card;
}

Card ZoneClass::takeBottomCard() {
	Card card = ordered_zone_.back();
	ordered_zone_.pop_back();
	zone_counts_[card] -= 1;
	return card;
}

bool ZoneClass::takeFirst(const Card& card) {
	for(std::deque<Card>::iterator it = ordered_zone_.begin(); it != ordered_zone_.end(); ++it) {
		if(*it == card) {
			ordered_zone_.erase(it);
			zone_counts_[card] -= 1;
			return true;
		}
	}
	return false;
}

bool ZoneClass::takeLast(const Card& card) {
	for(std::deque<Card>::reverse_iterator it = ordered_zone_.rbegin(); it != ordered_zone_.rend(); ++it) {
		if(*it == card) {
			ordered_zone_.erase((it+1).base());
			zone_counts_[card] -= 1;
			return true;
		}
	}
	return false;
}

void ZoneClass::putCardOnTop(const Card& card) {
	if(!contains(card)) {
		zone_counts_[card] = 0;
	}
	ordered_zone_.push_front(card);
	zone_counts_[card] += 1;
}

void ZoneClass::putCardOnBottom(const Card& card) {
	if(!contains(card)) {
		zone_counts_[card] = 0;
	}
	ordered_zone_.push_back(card);
	zone_counts_[card] += 1;
}

size_t ZoneClass::size() const {
	return ordered_zone_.size();
}

size_t ZoneClass::count(const Card& card) const {
	auto entry_it = zone_counts_.find(card);
	if(entry_it == zone_counts_.end())
		return 0;
	return entry_it->second;
}

bool ZoneClass::contains(const Card& card) const {
	return zone_counts_.find(card) != zone_counts_.end() && zone_counts_.at(card) > 0;
}


}
