#pragma once
#ifndef COMPILE_HPP
#define COMPILE_HPP

#include <vector>

#include "parse/node.hpp"

namespace nori {

std::vector<char>
compile(std::vector<nori::parse::Node> const &nodes);

} // namespace nori

#endif