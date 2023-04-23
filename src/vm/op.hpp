#pragma once
#ifndef OP_HPP
#define OP_HPP

#include <cstdint>

namespace nori::vm {

#define XNODES_TO_OP \
	X(PopNode, Pop) \
	X(NumericInNode, NumericIn) \
	X(InNode, In) \
	X(AsciiInNode, AsciiIn) \
	X(OutNode, Out) \
	X(AsciiOutNode, AsciiOut) \
	X(SwapNode, Swap) \
	X(ReverseNode, Reverse) \
	X(DupNode, Dup) \
	X(AddNode, Add) \
	X(SubNode, Sub) \
	X(DivNode, Div) \
	X(MulNode, Mul) \
	X(PowNode, Pow) \
	X(RootNode, Root) \
	X(ModNode, Mod) \
	X(CeilNode, Ceil) \
	X(FloorNode, Floor) \
	X(RandNode, Rand) \
	X(BitRandNode, BitRand) \
	X(ByteRandNode, ByteRand) \
	X(JumpBeginNode, JumpBegin)

#define X(_, Op) Op,

enum Op : std::uint8_t { Return, Push, PushString, XNODES_TO_OP };

#undef X

} // namespace nori::vm

#endif