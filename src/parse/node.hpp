#pragma once
#ifndef AST_HPP
#define AST_HPP

#include <variant>
#include <vector>

#include "token.hpp"

namespace nori::parse {

#define XNODES \
	X(Pop, PopNode) \
	X(In, InNode) \
	X(Out, OutNode) \
	X(Swap, SwapNode) \
	X(Reverse, ReverseNode) \
	X(Dup, DupNode) \
	X(Add, AddNode) \
	X(Sub, SubNode) \
	X(Div, DivNode) \
	X(Mul, MulNode) \
	X(Pow, PowNode) \
	X(Root, RootNode) \
	X(Mod, ModNode) \
	X(Ceil, CeilNode) \
	X(Floor, FloorNode) \
	X(Rand, RandNode) \
	X(BitRand, BitRandNode) \
	X(ByteRand, ByteRandNode) \
	X(JumpBegin, JumpBeginNode)

class PushNode;

#define X(_, NodeName) \
	class NodeName {};

XNODES

#undef X

class ConditionalNode;

#define X(_, NodeName) NodeName,

using Node = std::variant<PushNode, XNODES ConditionalNode>;

#undef X

class PushNode {

  public:
	NoriValue const value;
	PushNode(NoriValue const &value) : value{value} {}
};

class ConditionalNode {
	std::vector<Node> body;

  public:
	ConditionalNode(std::vector<Node> &&body) : body{std::move(body)} {}
};

} // namespace nori::parse

#endif