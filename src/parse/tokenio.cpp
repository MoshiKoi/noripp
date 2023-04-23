#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>

#include "tokenio.hpp"
#include "../utils.hpp"

std::ostream &
operator<<(std::ostream &os, nori::parse::Token const &tok) {
	switch (tok.type) {

#define X(Name, Symbol) \
	case nori::parse::TokenType::Name: os << #Name; break;

		XSYMBOLS

	case nori::parse::TokenType::Value:
		std::visit(
		    overloaded{
		        [&](float const &val) { os << "Float(" << val << ')'; },
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
		    tok.value.value());
		break;
	default: throw new std::runtime_error{"Unhandled token type"};
	}
	return os;
}

std::ostream &
operator<<(std::ostream &os, nori::parse::TokenType const &ty) {
	switch (ty) {
		XSYMBOLS
#undef X
	default: throw new std::runtime_error{"Unhandled token type"};
	}

	return os;
}