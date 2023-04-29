#include "test_utils.hpp"

int
test_io(int argc, char ** const argv) {
	return test_nori_program("IO", "Hello, World!", "Hello, World!");
}