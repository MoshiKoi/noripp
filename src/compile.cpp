#include <array>
#include <bit>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <variant>

#include "compile.hpp"
#include "utils.hpp"
#include "vm/op.hpp"

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
compile(std::vector<char> &result, parse::PushNode const &node) {
	std::visit(
	    overloaded{
	        [&](float const &value) {
		        result.emplace_back(vm::Op::Push);
		        insert_float(result, value);
	        },
	        [&](std::string const &value) {
		        result.emplace_back(vm::Op::PushString);
		        insert_string(result, value);
	        }},
	    node.value);
}

#define X(NodeName, OpName) \
	void compile(std::vector<char> &result, parse::NodeName const &node) { \
		result.emplace_back(vm::Op::OpName); \
	}

XNODES_TO_OP

#undef X

std::vector<char> compile_body(std::vector<parse::Node> const &nodes);

void
compile(std::vector<char> &result, parse::ConditionalNode const &node) {
	auto const inner = compile_body(node.body);

	result.emplace_back(vm::Op::ForwardJumpFalse);
	result.emplace_back(inner.size() + 2);
	result.insert(result.end(), inner.begin(), inner.end());
	result.emplace_back(vm::Op::BackwardJumpTrue);
	result.emplace_back(inner.size() + 2);
}

std::vector<char>
compile_body(std::vector<parse::Node> const &nodes) {
	std::vector<char> result{};

	for (auto const &node : nodes) {
		std::visit([&](auto &&x) { compile(result, x); }, node);
	}

	return result;
}

std::vector<char>
compile(std::vector<parse::Node> const &nodes) {
	auto result = compile_body(nodes);

	result.emplace_back(vm::Op::Return);

	return result;
}

} // namespace nori