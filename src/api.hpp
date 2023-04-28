#pragma once
#ifndef NORI_HPP
#define NORI_HPP

#include <iostream>

#include "parse/parse.hpp"

int
run_stream(std::istream &program, std::ostream &out = std::cout, std::istream& in = std::cin);

void
compile(std::string_view const &source, std::ostream &out);

#endif