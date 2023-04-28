#include "compile.hpp"
#include "parse/parse.hpp"
#include "parse/tokenio.hpp"
#include "parse/tokens.hpp"
#include "vm/vm.hpp"

int
run_stream(std::istream &program, std::ostream &output, std::istream& input) {
	nori::vm::VM vm{program, 255, output, input};
	try {
		vm.exec();
		return 0;
	} catch (nori::vm::InvalidOperandException) {
		std::cerr << "Attempted to operate on two invalid operands" << std::endl;
		return 1;
	} catch (nori::vm::StackException) {
		std::cerr << "Stack doesn't contain enough elements" << std::endl;
		return 1;
	} catch (std::runtime_error err) {
		std::cerr << err.what() << std::endl;
		return 1;
	}
}

void
compile(std::string_view const &source, std::ostream &out) {
	nori::parse::Tokens toks{source};

	auto iter = std::begin(toks);
	auto const end = std::end(toks);
	auto const nodes = nori::parse::parse(iter, end);

	auto const buffer = nori::compile(nodes);
	out.write(buffer.data(), buffer.size());
}