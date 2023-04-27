#include <cmath>
#include <variant>

#include "../utils.hpp"
#include "vm.hpp"

namespace nori::vm {

#define X(Name, Op, _) \
	void Name(NoriValue const&a, NoriValue &b) { \
		 std::visit( \
		    overloaded{ \
		        [&](double const &a_val, double const &b_val) -> void { b.emplace<double>(a_val Op b_val); }, \
		        [](auto, auto) -> void { throw InvalidOperandException{}; }}, \
		    a, b); \
	}

XBINOPS

NoriValue
mod(NoriValue const &a, NoriValue const &b) {
	return std::visit(
	    overloaded{
	        [](double const &a, double const &b) -> NoriValue { return a - std::floor(a / b) * b; },
	        [](auto, auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a, b);
}

NoriValue
pow(NoriValue const &a, NoriValue const &b) {
	return std::visit(
	    overloaded{
	        [](double const &a, double const &b) -> NoriValue { return std::pow(a, b); },
	        [](auto, auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a, b);
}

NoriValue
floor(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](double const &a) -> NoriValue { return std::floor(a); },
	        [](auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a);
}

NoriValue
ceil(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](double const &a) -> NoriValue { return std::ceil(a); },
	        [](auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a);
}

NoriValue
root(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](double const &a) -> NoriValue { return std::sqrt(a); },
	        [](auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a);
}

bool
truthy(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](double const &a) -> bool { return a != 0; },
			[](std::string const &s) -> bool { return true; },
	        [] (auto) -> bool { throw InvalidOperandException{}; }},
	    a);
}

#undef X

} // namespace nori::vm