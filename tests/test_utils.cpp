#include <sstream>

#include <fmt/core.h>

#include "../src/api.hpp"

#include "test_utils.hpp"

int
test_nori_program(std::string_view nori_code, std::string_view expected_output, std::string input) {
	std::stringstream ss{};
	compile(nori_code, ss);

	std::stringstream os{};
	std::stringstream is{input};
	run_stream(ss, os, is);

	if (os.view() != expected_output) {
		std::cerr << fmt::format("Expected:\n{}\nGot:\n{}", expected_output, os.view());
		return 1;
	} else {
		return 0;
	}
}