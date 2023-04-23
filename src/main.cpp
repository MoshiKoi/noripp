#include <cstddef>
#include <cstring>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include "compile.hpp"
#include "parse/parse.hpp"
#include "parse/tokenio.hpp"
#include "parse/tokens.hpp"
#include "vm/vm.hpp"

int
run_stream(std::istream &s) {
	nori::vm::VM vm{s, 255};
	try {
		vm.exec();
		return 0;
	} catch (nori::vm::InvalidOperandException) {
		std::cerr << "Attempted to operate on two invalid operands" << std::endl;
		return 1;
	} catch (std::runtime_error err) {
		std::cerr << err.what() << std::endl;
		return 1;
	}
}

std::vector<char>
compile(std::string_view const &source, bool debug = false) {
	nori::parse::Tokens toks{source};

	auto iter = std::begin(toks);
	auto const end = std::end(toks);
	auto const nodes = nori::parse::parse(iter, end);

	if (debug) {
		for (auto const &tok : toks)
			std::cout << tok << ' ';
		std::cout << '\n';
	}

	auto const buffer = nori::compile(nodes);
	return buffer;
}

int
run(int argc, char const **argv) {
	if (argc < 2) {
		std::cout << "File required\n";
		return 1;
	}

	auto filename = argv[1];
	std::ifstream fs{filename};
	return run_stream(fs);
}

int
build(int argc, char const **argv) {
	using namespace nori::vm;

	if (argc < 2) {
		std::cout << "File required\n";
		return 1;
	}

	std::string filename{argv[1]};
	std::ifstream fs{filename};

	std::size_t ext_index = filename.find_last_of('.');
	if (ext_index == std::string::npos)
		ext_index = filename.size();

	std::size_t const ext_len = filename.size() - ext_index;
	filename.replace(ext_index, ext_len, ".nr");

	std::stringstream sourcestream{};
	sourcestream << fs.rdbuf();
	std::string const source = sourcestream.str();

	try {
		auto const buffer = compile(source);
		std::ofstream bfs{filename, std::ios_base::binary | std::ios_base::trunc};
		bfs.write(buffer.data(), buffer.size());
	} catch (nori::parse::UnexpectedTokenError err) {
		std::cerr << "Unexpected token: " << err.actual << ", expected ";
		for (auto const &expected : err.expected)
			std::cerr << expected << ',';
		std::cerr << '\n';
		return 1;
	} catch (std::runtime_error err) {
		std::cerr << err.what() << '\n';
		return 1;
	}

	std::cout << "Compiled to " << filename << '\n';
	return 0;
}

int
exec(int argc, char const **argv) {
	std::string source;

	if (argc < 2) {
		std::stringstream sourcestream{};
		sourcestream << std::cin.rdbuf();
		source = sourcestream.str();
	} else {
		std::stringstream sourcestream{};
		std::ifstream fs{argv[1]};
		sourcestream << fs.rdbuf();
		source = sourcestream.str();
	}

	std::stringstream ss{};

	try {
		auto const buffer = compile(source);
		ss.write(buffer.data(), buffer.size());
	} catch (nori::parse::UnexpectedTokenError err) {
		std::cerr << "Unexpected token: " << err.actual << ", expected ";
		for (auto const &expected : err.expected)
			std::cerr << expected << ',';
		std::cerr << '\n';
		return 1;
	} catch (std::runtime_error err) {
		std::cerr << err.what() << '\n';
		return 1;
	}

	return run_stream(ss);
}

int
main(int argc, char const **argv) {
	if (argc > 1) {
		auto sub = argv[1];

		if (std::strcmp("run", sub) == 0)
			return run(argc - 1, argv + 1);

		if (std::strcmp("build", sub) == 0)
			return build(argc - 1, argv + 1);

		if (std::strcmp("exec", sub) == 0)
			return exec(argc - 1, argv + 1);
	}

	std::cout << "Usage:\n"
	             "\tnori run [file]\n"
	             "\tnori build [file]\n";
	return 1;
}