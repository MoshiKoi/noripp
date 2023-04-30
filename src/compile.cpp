#include <array>
#include <bit>
#include <limits>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include "compile.hpp"
#include "utils.hpp"
#include "vm/op.hpp"

namespace nori {

using IdentifierMap = std::unordered_map<std::string, std::uint8_t>;

void
insert_double(std::vector<char> &buffer, double x) {
	auto const arr = std::bit_cast<std::array<char, 8>>(x);
	buffer.insert(buffer.end(), arr.begin(), arr.end());
}

void
insert_string(std::vector<char> &buffer, std::string const &value) {
	buffer.insert(buffer.end(), value.begin(), value.end());
	buffer.emplace_back(0);
}

std::uint8_t
get_or_new_id(IdentifierMap &vars, std::string const &name) {
	auto const res = vars.find(name);
	if (res != vars.end()) {
		return res->second;
	} else {
		const std::size_t index = vars.size();
		if (index > std::numeric_limits<std::uint8_t>::max()) {
			throw std::runtime_error{"Limit on number of variables reached"};
		}
		vars.insert(std::pair{name, index});
		return index;
	}
}

void
compile(std::vector<char> &result, parse::PushNode const &node, IdentifierMap &) {
	std::visit(
	    overloaded{
	        [&](double const &value) {
		        result.emplace_back(vm::Op::Push);
		        insert_double(result, value);
	        },
	        [&](std::string const &value) {
		        result.emplace_back(vm::Op::PushString);
		        insert_string(result, value);
	        }},
	    node.value);
}

void
compile(std::vector<char> &result, parse::PushVar const &node, IdentifierMap &vars) {
	result.emplace_back(vm::Op::PushVar);
	result.emplace_back(get_or_new_id(vars, node.name));
}

void
compile(std::vector<char> &result, parse::SetVarPop const &node, IdentifierMap &vars) {
	result.emplace_back(vm::Op::SetVarPop);
	result.emplace_back(get_or_new_id(vars, node.name));
}

void
compile(std::vector<char> &result, parse::SetVarValue const &node, IdentifierMap &vars) {
	// ? Note: not very efficient, but it means the vm doesn't need more instructions
	compile(result, parse::PushNode{node.value}, vars);
	compile(result, parse::SetVarPop{node.name}, vars);
}

#define X(NodeName, OpName) \
	void compile(std::vector<char> &result, parse::NodeName const &node, IdentifierMap &) { \
		result.emplace_back(vm::Op::OpName); \
	}

XNODES_TO_OP

#undef X

void compile_body(std::vector<char> &result, std::vector<parse::Node> const &nodes, IdentifierMap &vars);

void
compile(std::vector<char> &result, parse::ConditionalNode const &node, IdentifierMap &vars) {
	std::vector<char> inner{};
	compile_body(inner, node.body, vars);

	result.emplace_back(vm::Op::ForwardJumpFalse);
	result.emplace_back(inner.size() + 2);
	result.insert(result.end(), inner.begin(), inner.end());
	result.emplace_back(vm::Op::BackwardJumpTrue);
	result.emplace_back(inner.size() + 2);
}

void
compile_body(std::vector<char> &result, std::vector<parse::Node> const &nodes, IdentifierMap &vars) {
	for (auto const &node : nodes) {
		std::visit([&](auto &&x) { compile(result, x, vars); }, node);
	}
}

std::vector<char>
compile(std::vector<parse::Node> const &nodes) {
	IdentifierMap vars{};
	std::vector<char> result{};
	result.emplace_back(0);
	compile_body(result, nodes, vars);
	result.front() = vars.size();
	result.emplace_back(vm::Op::Return);

	return result;
}

} // namespace nori