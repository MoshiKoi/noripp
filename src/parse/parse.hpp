#pragma once
#ifndef PARSE_HPP
#define PARSE_HPP

#include <exception>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <vector>

#include "node.hpp"
#include "token.hpp"
#include "tokenio.hpp"


namespace nori::parse {

class UnexpectedEndOfInput {
  public:
	std::vector<TokenType> const expected;
	UnexpectedEndOfInput(std::vector<TokenType> &&expected) : expected{std::move(expected)} {}
};

class UnexpectedTokenError {
  public:
	Token const actual;
	std::vector<TokenType> const expected;
	UnexpectedTokenError(std::vector<TokenType> &&expected, Token const &actual)
	    : actual{actual}, expected{std::move(expected)} {}
};

template <std::input_iterator Iter>
PushNode
parse_push(Iter &iter, Iter const &end) {
	if (iter == end)
		throw std::runtime_error{"Unexpected end of input"};

	auto const tok = *iter;

	if (tok.type == TokenType::Value) {
		++iter;
		return PushNode{tok.value};
	}

	throw UnexpectedTokenError({TokenType::Value}, tok);
}

template <std::input_iterator Iter>
std::vector<Node>
parse(Iter &iter, Iter const &end) {
	std::vector<Node> nodes{};

	while (iter != end) {
		Token const tok = *iter;
		switch (tok.type) {
		case TokenType::Push:
			++iter;
			nodes.emplace_back(parse_push(iter, end));
			break;
		case TokenType::LBracket:
			++iter;
			nodes.emplace_back(ConditionalNode{parse(iter, end)});
			if (iter == end) {
				throw UnexpectedEndOfInput{{TokenType::RBracket}};
			} else if ((*iter).type != TokenType::RBracket) {
				throw UnexpectedTokenError{{TokenType::RBracket}, *iter};
			}
			++iter;
			break;
		case TokenType::RBracket: goto end;

#define X(TokenName, NodeName) \
	case TokenType::TokenName: \
		++iter; \
		nodes.emplace_back(NodeName{}); \
		break;

			XNODES
#undef X
		default:
#define X(TokenName, _) TokenType::TokenName,
			throw UnexpectedTokenError{{TokenType::Push, XNODES}, tok};
#undef X
		}
	}
end:

	return nodes;
}

} // namespace nori::parse

#endif