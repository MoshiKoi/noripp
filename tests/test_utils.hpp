#pragma once
#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <string>
#include <string_view>

int test_nori_program(std::string_view nori_code, std::string_view expected_output, std::string input = "");

#endif