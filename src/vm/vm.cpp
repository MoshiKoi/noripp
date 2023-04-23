#include <cmath>
#include <variant>

#include "vm.hpp"

template <class... Ts>
struct overloaded : Ts... {
	using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace nori::vm {

#define X(Name, Op) \
	NoriValue Name(NoriValue const &a, NoriValue const &b) { \
		return std::visit( \
		    overloaded{ \
		        [](float const &a, float const &b) -> NoriValue { return a Op b; }, \
		        [](auto, auto) -> NoriValue { throw InvalidOperandException{}; }}, \
		    a, b); \
	}

XBINOPS

NoriValue
mod(NoriValue const &a, NoriValue const &b) {
	return std::visit(
	    overloaded{
	        [](float const &a, float const &b) -> NoriValue {
		        // TODO: Actual validation or smth
		        return (float)((int)a % (int)b);
	        },
	        [](auto, auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a, b);
}

NoriValue
pow(NoriValue const &a, NoriValue const &b) {
	return std::visit(
	    overloaded{
	        [](float const &a, float const &b) -> NoriValue { return std::pow(a, b); },
	        [](auto, auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a, b);
}

NoriValue
floor(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](float const &a) -> NoriValue { return std::floor(a); },
	        [](auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a);
}

NoriValue
ceil(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](float const &a) -> NoriValue { return std::ceil(a); },
	        [](auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a);
}

NoriValue
root(NoriValue const &a) {
	return std::visit(
	    overloaded{
	        [](float const &a) -> NoriValue { return std::sqrt(a); },
	        [](auto) -> NoriValue { throw InvalidOperandException{}; }},
	    a);
}

#undef X

} // namespace nori::vm