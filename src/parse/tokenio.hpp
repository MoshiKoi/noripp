#pragma once
#ifndef TOKENIO_HPP
#define TOKENIO_HPP

#include <ostream>

#include "token.hpp"

std::ostream &operator<<(std::ostream &os, nori::parse::Token const &tok);

std::ostream &operator<<(std::ostream &os, nori::parse::TokenType const &ty);

#endif