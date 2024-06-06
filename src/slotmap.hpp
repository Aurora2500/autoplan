#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

namespace autoplan {
namespace dsa {

struct key {
	std::int32_t index;
	std::int32_t generation;

	inline std::uint64_t id() const {
		return (std::uint64_t)generation << 32 | index;
	}

	inline key(std::uint64_t id) 
		: index(id & 0xFFFFFFFF)
		, generation(id >> 32)
	{}

	inline key(std::int32_t index, std::int32_t generation)
		: index(index)
		, generation(generation)
	{}

	inline bool operator==(const key& other) const {
		return index == other.index && generation == other.generation;
	}

	inline bool operator!=(const key& other) const {
		return index != other.index || generation != other.generation;
	}

	inline bool operator<(const key& other) const {
		return id() < other.id();
	}

	inline bool operator>(const key& other) const {
		return id() > other.id();
	}

	inline bool operator<=(const key& other) const {
		return id() <= other.id();
	}

	inline bool operator>=(const key& other) const {
		return id() >= other.id();
	}
};

template <typename T>
struct slot {
	std::int32_t generation;
	union {
		T value;
		std::int32_t next;
	};

	inline slot()
		: generation(1)
		, next(-1)
	{}

	inline slot(T&& value)
		: generation(0)
		, value(value)
	{}

	inline slot(const slot<T>& other)
		: generation(other.generation)
	{
		if (generation % 2 == 0) {
			if constexpr (std::is_destructible_v<T>) {
				value.~T();
			}
			value = T(other.value);
		} else {
			next = other.next;
		}
	}

	inline slot<T>& operation=(const slot<T>& other) {
		if (this == &other) {
			return *this;
		}

		if constexpr (std::is_destructible_v<T>) {
			if (generation % 2 == 0) {
				value.~T();
			}
		}

		generation = other.generation;
		if (generation % 2 == 0) {
			value = T(other.value);
		} else {
			next = other.next;
		}
	}

	inline slot(slot<T>&& other)
		: generation(other.generation)
	{
		if (generation % 2 == 0) {
			value = std::move(other.value);
		} else {
			next = other.next;
		}
	}

	inline slot<T>& operator=(slot<T>&& other) {
		if (this == &other) {
			return *this;
		}

		if constexpr (std::is_destructible_v<T>) {
			if (generation % 2 == 0) {
				value.~T();
			}
		}

		generation = other.generation;
		if (generation % 2 == 0) {
			value = std::move(other.value);
		} else {
			next = other.next;
		}
	}

	inline ~slot() {
		if constexpr (std::is_destructible_v<T>) {
			if (generation % 2 == 0) {
				value.~T();
			}
		}
	}
};

template <typename T>
class slotmap {
private:
	std::uint32_t m_head;
	std::vector<slot<T>> m_slots;

public:
	inline slotmap()
		: m_head(-1)
	{}

	slotmap(const slotmap<T>& other) = delete;

	inline key insert_with_key(std::function<T(key)> ctor) {
		if (m_head == -1) {
			key k = key(m_slots.size(), 0);
			m_slots.emplace_back(ctor(k));
			return k;
		}

		slot<T>& s = m_slots[m_head];
		auto idx = m_head;
		auto gen = ++s.generation;
		m_head = s.next;
		key k(idx, gen);

		s.value = ctor(k);

		return k;
	}


	inline key insert(const T& value) {
		return insert_with_key([value]{return value;});
	}

	inline T& operator[](const key& k) {
		if (k.index >= m_slots.size()) {
			throw std::out_of_range("Index out of range");
		}
		slot<T>& s = m_slots[k.index];
		if (s.generation != k.generation) {
			throw std::out_of_range("Generation mismatch");
		}
		return s.value;
	}

	inline bool erase(const key& k) {
		if (k.index >= m_slots.size()) {
			return false;
		}

		slot<T>& s = m_slots[k.index];
		if (s.generation != k.generation) {
			return false;
		}
		s.generation++;
		s.next = m_head;
		s.value.~T();
		return true;
	}
};

}
}