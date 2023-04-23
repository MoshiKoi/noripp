#include <array>
#include <bit>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <variant>

#include "compile.hpp"
#include "vm/op.hpp"

template <class... Ts>
struct overloaded : Ts... {
	using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace nori {

void
insert_float(std::vector<char> &buffer, float x) {
	auto const arr = std::bit_cast<std::array<char, 4>>(x);
	buffer.insert(buffer.end(), arr.begin(), arr.end());
}

void
insert_string(std::vector<char> &buffer, std::string const &value) {
	buffer.insert(buffer.end(), value.begin(), value.end());
	buffer.emplace_back(0);
}

void
compile(std::vector<char> &result, parse::PushNode node) {
	// clang-format off
    std::visit(overloaded {
        [&](float const &value) {
            result.emplace_back(vm::Op::Push);
            insert_float(result, value);
        },
        [&](std::string const &value) {
            result.emplace_back(vm::Op::PushString);
            insert_string(result, value);
        }},
	    node.value);
	// clang-format on
}

#define X(NodeName, OpName) \
	void compile(std::vector<char> &result, parse::NodeName node) { \
		result.emplace_back(vm::Op::OpName); \
	}

XNODES_TO_OP

#undef X

void
compile(std::vector<char> &result, parse::ConditionalNode node) {
	throw std::runtime_error{"Conditional is not implemented"};
}

std::vector<char>
compile(std::vector<parse::Node> const &nodes) {
	std::vector<char> result{};

	for (auto const &node : nodes) {
		std::visit([&](auto &&x) { compile(result, x); }, node);
	}

	result.emplace_back(vm::Op::Return);

	return result;
}

} // namespace nori