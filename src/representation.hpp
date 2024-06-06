#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "slotmap.hpp"

namespace autoplan {

class internal;
class type;
class symbol;
class predicate;

class domain {
private:
	std::shared_ptr<internal> m_internal;

	domain(const std::shared_ptr<internal>& internal);
public:
	domain();

	bool operator==(const domain& other) const;

	type new_type();
	type new_type(const std::string& name);

	symbol new_symbol();
	symbol new_symbol(const std::string& name);
	std::vector<symbol> symbols() const;

	predicate new_predicate(int arity);
	predicate new_predicate(const std::string& name, int arity);
	std::vector<predicate> predicates() const;

	friend class symbol;
	friend class type;
	friend class state;
};

class type {
private:
	dsa::key m_key;
	std::shared_ptr<internal> m_internal;

public:
	type(dsa::key m_key, const std::shared_ptr<internal>& m_internal);
	const std::string& name() const;

	domain get_domain() const;
};


class symbol {
private:
	dsa::key m_key;
	std::shared_ptr<internal> m_internal;

public:
	symbol(dsa::key m_key, const std::shared_ptr<internal>& internal);

	bool operator==(const symbol& other) const;
	bool operator!=(const symbol& other) const;
	bool operator<(const symbol& other) const;
	bool operator>(const symbol& other) const;
	bool operator<=(const symbol& other) const;
	bool operator>=(const symbol& other) const;

	const std::string& name() const;

	domain get_domain() const;
	std::optional<type> get_type() const;

	friend class std::hash<autoplan::symbol>;
	friend class domain;
};

class predicate {
private:
	dsa::key m_key;
	std::shared_ptr<internal> m_internal;

public:
	predicate(dsa::key m_key, const std::shared_ptr<internal>& internal);
	const std::string& name() const;
	int arity() const;

	friend class domain;
};

class operator_element {
private:
	std::weak_ptr<predicate> m_predicate;
	std::vector<int> m_bindings;
	bool m_positive;
};

class operation {
private:
	std::uint64_t m_id;
	std::string m_name;
	int m_arity;
	std::weak_ptr<internal> m_internal;

	std::vector<operator_element> m_preconditions;
};

class internal_type {
private:
	dsa::key m_id;
	std::string m_name;

public:
	friend class type;
};

class internal_symbol {
private:
	dsa::key m_id;
	std::string m_name;
	std::optional<dsa::key> m_type;

public:
	internal_symbol(dsa::key id, const std::string& name);
	internal_symbol(dsa::key id, const std::string& name, dsa::key type);
	friend class symbol;
};

class internal_predicate {
private:
	dsa::key m_id;
	std::string m_name;
	int m_arity;

public:
internal_predicate(dsa::key id, const std::string& name, int arity);
	friend class predicate;
};

class internal {
private:
	dsa::slotmap<internal_type> m_types;
	dsa::slotmap<internal_symbol> m_symbols;
	dsa::slotmap<internal_predicate> m_predicates;

public:
	internal() = default;

	friend class domain;
	friend class type;
	friend class symbol;
	friend class predicate;
};

class ground_predicate {
private:
	std::shared_ptr<predicate> m_predicate;
	std::vector<std::shared_ptr<symbol>> m_arguments;

public:
	ground_predicate(const std::shared_ptr<predicate>& p, std::initializer_list<std::shared_ptr<symbol>> arguments);
	ground_predicate(const std::shared_ptr<predicate>& p, const std::vector<std::shared_ptr<symbol>>& arguments);
};

class state {
private:
	std::shared_ptr<internal> m_internal;
	std::vector<ground_predicate> m_predicates;

public:
	state(const domain& d);

	void add_predicate(const std::shared_ptr<predicate>& p, std::initializer_list<std::shared_ptr<symbol>> arguments);
	void add_predicate(const std::shared_ptr<predicate>& p, const std::vector<std::shared_ptr<symbol>>& arguments);

};

}

template<>
class std::hash<autoplan::symbol> {
public:
	std::size_t operator()(const autoplan::symbol& s) const;
};