#pragma once
#include <string>
#include <ostream>
#include <map>

namespace MTGSims {

class Card {
private:
	std::string name_;
	size_t id_;

	static size_t id_counter_;
	static std::map<std::string, size_t> id_map_;

public:
	Card(std::string name);
	Card() : Card("Nameless card") {}
	size_t getId() const;
	friend std::ostream& operator<<(std::ostream& out, const Card& card);
	friend bool operator==(const Card& lhs, const Card& rhs);
	friend bool operator<(const Card& lhs, const Card& rhs);
};



}
