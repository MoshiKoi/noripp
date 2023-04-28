#pragma once
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <optional>

#include "../common.hpp"

namespace nori::parse {

#define XSYMBOLS \
	X(Push, '>') \
	X(Pop, '<') \
	X(NumericIn, 'N') \
	X(In, 'I') \
	X(AsciiIn, ',') \
	X(Out, 'O') \
	X(AsciiOut, '.') \
	X(Swap, '@') \
	X(Reverse, '$') \
	X(Dup, ':') \
	X(Add, '+') \
	X(Sub, '-') \
	X(Div, '/') \
	X(Mul, '*') \
	X(Pow, '^') \
	X(Root, 'z') \
	X(Mod, '%') \
	X(Ceil, 'c') \
	X(Floor, 'f') \
	X(Rand, 'r') \
	X(BitRand, 'b') \
	X(ByteRand, 'B') \
	X(JumpBegin, 'W') \
	X(LBracket, '[') \
	X(RBracket, ']') \
	X(Question, '?') \

#define X(Name, Symbol) Name,

enum class TokenType { Value, Error, XSYMBOLS };

#undef X

struct Token {
	TokenType type;
	std::optional<NoriValue> value;
};

} // namespace nori::parse

#endif