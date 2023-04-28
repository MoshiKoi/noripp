#include "test_utils.hpp"

int
test_stack_reversal(int argc, char ** const argv) {
	return test_nori_program(">1>2>3 $ OOO", "123");
}