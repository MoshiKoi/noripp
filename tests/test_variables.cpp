#include "test_utils.hpp"
#include <array>
#include <ranges>

int
test_variables(int argc, char **const argv) {
	return (
	    test_nori_program("|my variable|1 >|my variable|O", "1") +
	    test_nori_program(">0>1 |my variable|< O >|my variable|O", "01"));
}