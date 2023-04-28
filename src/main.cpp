#include <cstddef>
#include <cstring>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include <fmt/ranges.h>

#include "api.hpp"

int
run(int argc, char const **argv) {
	if (argc < 2) {
		fmt::print("File required\n");
		return 1;
	}

	auto filename = argv[1];
	std::ifstream fs{filename};
	return run_stream(fs);
}

int
build(int argc, char const **argv) {
	if (argc < 2) {
		fmt::print("File required\n");
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
		std::ofstream bfs{filename, std::ios_base::binary | std::ios_base::trunc};
		compile(source, bfs);
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

	fmt::print("Compiled to {}\n", filename);
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
		compile(source, ss);
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

	fmt::print("Usage:\n"
	           "\tnori run [file]\n"
	           "\tnori build [file]\n"
	           "\tnori exec [file]\n");
	return 1;
}