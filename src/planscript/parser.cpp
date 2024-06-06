#include "parser.hpp"

#include <iostream>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace autoplan {
namespace planscript {

enum class token_type {
	keyword,
	identifier,
	colon,
	bracket_open,
	bracket_close,
	paren_open,
	paren_close,
	negation,
};

std::string token_name(const token_type& t) {
	switch (t)
	{
	case token_type::keyword:
		return "KEYWORD";
	case token_type::identifier:
		return "IDENTIFIER";
	case token_type::colon:
		return "COLON";
	case token_type::bracket_open:
		return "BRACKET OPEN";
	case token_type::bracket_close:
		return "BRACKET CLOSE";
	case token_type::paren_open:
		return "PAREN OPEN";
	case token_type::paren_close:
		return "PAREN CLOSE";
	case token_type::negation:
		return "NEGATION";
	default:
		return "UNKNOWN";
	}
}

struct token {
	token_type type;
	std::string value;
};

const std::unordered_set<std::string> keywords = {
	"types",
	"symbols",
	"predicates",
	"actions",
};

const std::unordered_map<char, token_type> token_map = {
	{':', token_type::colon},
	{'{', token_type::bracket_open},
	{'}', token_type::bracket_close},
	{'(', token_type::paren_open},
	{')', token_type::paren_close},
	{'!', token_type::negation},
};

std::vector<token> tokenize(const std::string& input) {
	std::vector<token> tokens;

	int i = 0;

	while (i < input.length()) {
		char c = input[i];
		if (std::isspace(c)) {
			i++;
			continue;
		}

		if (auto potential_token = token_map.find(c); potential_token != token_map.end()) {
			tokens.push_back(token{.type=potential_token->second});
			i++;
			continue;
		}

		if (std::isalpha(c)) {
			int length = 1;

			while(((i + length) < input.length()) && (c = input[i + length], isalpha(c))) {
				length++;
			}
			auto word = input.substr(i, length);
			bool is_keyword = keywords.find(word) != keywords.end();
			tokens.push_back(token {
				.type= is_keyword?token_type::keyword : token_type::identifier,
				.value = word
			});
			i += length;
			continue;
		}
	}

	return tokens;
}

domain parse(const std::string& input) {
	domain d;

	auto tokens = tokenize(input);

	for (auto token : tokens) {
		std::cout << token_name(token.type) << " " << token.value << std::endl;
	}

	return d;
}

}
}
