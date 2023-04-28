#include "test_utils.hpp"

int
test_hello_world(int argc, char ** const argv) {
	return test_nori_program(">'Hello, World!'O", "Hello, World!");
}