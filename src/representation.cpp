#include "representation.hpp"

#include <stdexcept>

namespace autoplan {

domain::domain(const std::shared_ptr<internal>& internal) : m_internal(internal) {}

domain::domain() : m_internal(std::make_shared<internal>()) {}

bool domain::operator==(const domain& other) const {
	return m_internal == other.m_internal;
}

symbol domain::new_symbol() {
	return new_symbol("unnamed_symbol");
}

symbol domain::new_symbol(const std::string& name) {
	auto key = m_internal->m_symbols.insert_with_key([name](dsa::key k){
		return internal_symbol(k, name);
	});
	return symbol(key, m_internal);
}

std::vector<symbol> domain::symbols() const {
	return {};
}

predicate domain::new_predicate(int arity) {
	return new_predicate("unnamed_predicate", arity);
}

predicate domain::new_predicate(const std::string& name, int arity) {
	auto key = m_internal->m_predicates.insert_with_key([name, arity](dsa::key k){
		return internal_predicate(k, name, arity);
	});
	return predicate(key, m_internal);
}

std::vector<predicate> domain::predicates() const {
	return {};
}

type::type(dsa::key key, const std::shared_ptr<internal>& internal)
	: m_key(key)
	, m_internal(internal)
{}

const std::string& type::name() const {
	return m_internal->m_types[m_key].m_name;
}


symbol::symbol(dsa::key key, const std::shared_ptr<internal>& internal)
	: m_key(key)
	, m_internal(internal)
{}

bool symbol::operator==(const symbol& other) const {
	return (m_key == other.m_key) && (m_internal == other.m_internal);
}

bool symbol::operator!=(const symbol& other) const {
	return m_key != other.m_key || m_internal != other.m_internal;
}

bool symbol::operator<(const symbol& other) const {
	return m_key < other.m_key;
}

bool symbol::operator>(const symbol& other) const {
	return m_key > other.m_key;
}

bool symbol::operator<=(const symbol& other) const {
	return m_key <= other.m_key;
}

bool symbol::operator>=(const symbol& other) const {
	return m_key >= other.m_key;
}

const std::string& symbol::name() const {
	return m_internal->m_symbols[m_key].m_name;
}

autoplan::domain symbol::get_domain() const {
	return domain(m_internal);
}

std::optional<autoplan::type> symbol::get_type() const {
	auto type_key = m_internal->m_symbols[m_key].m_type;
	if (type_key.has_value()) {
		return type(type_key.value(), m_internal);
	}	
	return std::nullopt;
}

predicate::predicate(dsa::key key, const std::shared_ptr<internal>& internal)
	: m_key(key)
	, m_internal(internal)
{}

const std::string& predicate::name() const {
	return m_internal->m_predicates[m_key].m_name;
}

int predicate::arity() const {
	return m_internal->m_predicates[m_key].m_arity;
}

internal_symbol::internal_symbol(dsa::key key, const std::string& name)
	: m_id(key)
	, m_name(name)
{}

internal_predicate::internal_predicate(dsa::key key, const std::string& name, int arity)
	: m_id(key)
	, m_name(name)
	, m_arity(arity)
{}

ground_predicate::ground_predicate(const std::shared_ptr<predicate>& predicate, std::initializer_list<std::shared_ptr<symbol>> arguments)
	: m_predicate(predicate)
	, m_arguments(arguments)
{
	if (predicate->arity() != m_arguments.size()) {
		throw std::invalid_argument("Arity mismatch");
	}
}

state::state(const domain& domain) : m_internal(domain.m_internal) {}

void state::add_predicate(const std::shared_ptr<predicate>& p, std::initializer_list<std::shared_ptr<symbol>> arguments) {
	m_predicates.push_back({p, arguments});
}

}

std::size_t std::hash<autoplan::symbol>::operator()(const autoplan::symbol& s) const {
	return std::hash<std::uint64_t>()(s.m_key.id());
}