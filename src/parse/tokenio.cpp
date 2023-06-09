#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>

#include "../utils.hpp"
#include "tokenio.hpp"

std::ostream &
operator<<(std::ostream &os, nori::parse::Token const &tok) {
	switch (tok.type) {

#define X(Name, Symbol) \
	case nori::parse::TokenType::Name: os << #Name; break;

		XSYMBOLS

	case nori::parse::TokenType::Value:
		std::visit(
		    overloaded{
		        [&](double const &val) { os << "double(" << val << ')'; },
		        [&](std::string const &val) {
			        os << "String(\"";
			        for (auto const &c : val) {
				        switch (c) {
				        case '\n': os << "\\n"; break;
				        case '\t': os << "\\t"; break;
				        default: os << c; break;
				        }
			        }
			        os << "\")";
		        }},
		    tok.value);
		break;
	case nori::parse::TokenType::Identifier:
		std::visit(
		    overloaded{
		        [&](double const &val) { throw std::runtime_error{"Identifier should have a string value"}; },
		        [&](std::string const &val) { os << "Var(" << val << ')'; }},
		    tok.value);
		break;
	case nori::parse::TokenType::Error: os << "Error"; break;
	default: throw new std::runtime_error{"Unhandled token type"};
	}
	return os;
}

std::ostream &
operator<<(std::ostream &os, nori::parse::TokenType const &ty) {
	switch (ty) {
		XSYMBOLS
#undef X
	case nori::parse::TokenType::Value: os << "Value"; break;
	case nori::parse::TokenType::Identifier: os << "Identifier"; break;
	case nori::parse::TokenType::Error: os << "Error"; break;
	default: throw new std::runtime_error{"Unhandled token type"};
	}

	return os;
}