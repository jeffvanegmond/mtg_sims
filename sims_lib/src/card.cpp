#include "card.h"
#include <ostream>

namespace MTGSims {

size_t Card::id_counter_ = 0;

Card::Card(std::string name) : name_(name) {
	if(Card::id_map_.find(name) == id_map_.end() ){
		id_ = Card::id_counter_++;
		Card::id_map_[name] = id_;
	}
	id_ = Card::id_map_[name];
}

size_t Card::getId() const {
	return id_;
}

std::ostream& operator<<(std::ostream& out, const Card& card) {
	return out << card.name_;
}

bool operator==(const Card& lhs, const Card& rhs) {
	return lhs.id_ == rhs.id_;
}

bool operator<(const Card& lhs, const Card& rhs) {
	return lhs.id_ < rhs.id_;
}

}
