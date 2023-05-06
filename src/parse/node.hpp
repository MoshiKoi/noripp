#pragma once
#ifndef AST_HPP
#define AST_HPP

#include <variant>
#include <vector>

#include "token.hpp"

namespace nori::parse {

#define XNODES \
	X(Pop, PopNode) \
	X(NumericIn, NumericInNode) \
	X(In, InNode) \
	X(AsciiIn, AsciiInNode) \
	X(Out, OutNode) \
	X(AsciiOut, AsciiOutNode) \
	X(Swap, SwapNode) \
	X(Reverse, ReverseNode) \
	X(Bury, BuryNode) \
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
class PushVar;
class SetVarPop;
class SetVarValue;
class ConditionalNode;

#define X(_, NodeName) \
	class NodeName {};

XNODES

#undef X

#define X(_, NodeName) NodeName,

using Node = std::variant<XNODES PushNode, PushVar, SetVarPop, SetVarValue, ConditionalNode>;

#undef X

class PushNode {

  public:
	NoriValue const value;
	PushNode(NoriValue const &value) : value{value} {}
};

class PushVar {
  public:
	std::string const name;
	PushVar(std::string const &name) : name{name} {}
};

class SetVarPop {
  public:
	std::string const name;
	SetVarPop(std::string const &name) : name{name} {}
};

class SetVarValue {
  public:
	std::string const name;
	NoriValue const value;
	SetVarValue(std::string const &name, NoriValue const &value) : name{name}, value{value} {}
};

class ConditionalNode {

  public:
	std::vector<Node> body;
	ConditionalNode(std::vector<Node> &&body) : body{std::move(body)} {}
};

} // namespace nori::parse

#endif