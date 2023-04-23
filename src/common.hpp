#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <variant>

#include "utils.hpp"

namespace nori {

using NoriValue = std::variant<float, std::string>;

}

#endif