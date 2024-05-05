#include "symbol.hpp"

namespace autoplan {

symbol::symbol(std::uint64_t id) : m_id(id) {}

bool symbol::operator==(const symbol& other) const {
	return m_id == other.m_id;
}

bool symbol::operator!=(const symbol& other) const {
	return m_id != other.m_id;
}

bool symbol::operator<(const symbol& other) const {
	return m_id < other.m_id;
}

bool symbol::operator>(const symbol& other) const {
	return m_id > other.m_id;
}

bool symbol::operator<=(const symbol& other) const {
	return m_id <= other.m_id;
}

bool symbol::operator>=(const symbol& other) const {
	return m_id >= other.m_id;
}

counter::counter() : m_counter(0) {}

symbol counter::next() {
	return symbol(m_counter++);
}

}