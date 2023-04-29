#include "test_utils.hpp"

int
test_comments(int argc, char ** const argv) {
	return test_nori_program(">'printed'O~~ >'shouldn't be printed'O ~~", "printed");
}