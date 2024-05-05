#pragma once
#include <cstdint>

namespace autoplan {

class symbol {
private:
	std::uint64_t m_id;
	symbol(std::uint64_t id);
public:

	bool operator==(const symbol& other) const;
	bool operator!=(const symbol& other) const;
	bool operator<(const symbol& other) const;
	bool operator>(const symbol& other) const;
	bool operator<=(const symbol& other) const;
	bool operator>=(const symbol& other) const;

	friend class counter;
};

class counter {
private:
	std::uint64_t m_counter;
public:
	counter();

	symbol next();
};

}