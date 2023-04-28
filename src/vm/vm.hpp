#pragma once
#ifndef NORIVM_HPP
#define NORIVM_HPP

#include <array>
#include <concepts>
#include <cstdint>
#include <deque>
#include <iostream>
#include <istream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>

#include <fmt/core.h>

#include "../common.hpp"
#include "op.hpp"

namespace nori::vm {

static_assert(std::numeric_limits<double>::is_iec559, "doubleing point format must be IEEE binary32");
static_assert(CHAR_BIT == 8, "Bytecode requires chars to be 8 bits");

class InvalidOperandException {};
class StackException {};

#define XBINOPS \
	X(Op::Add, [](double const &a, double const &b) { return a + b; }) \
	X(Op::Sub, [](double const &a, double const &b) { return a - b; }) \
	X(Op::Mul, [](double const &a, double const &b) { return a * b; }) \
	X(Op::Div, [](double const &a, double const &b) { return a / b; }) \
	X(Op::Pow, [](double const &a, double const &b) { return std::pow(a, b); }) \
	X(Op::Mod, [](double const &a, double const &b) { return a - std::floor(a / b) * b; })

NoriValue floor(NoriValue const &);
NoriValue ceil(NoriValue const &);
NoriValue root(NoriValue const &);
bool truthy(NoriValue const &);

template <std::derived_from<std::basic_istream<char>> T>
class VM {
  public:
	VM(T &stream, std::size_t buffer_size)
	    : _stream{stream}, _buffer{new std::uint8_t[buffer_size]}, _buffer_size{buffer_size}, _buffer_offset{0},
	      _ip{_buffer}, _stack{}, _reversed{false} {
		load(0);
	}
	~VM() { delete[] _buffer; }
	void exec() {
		while (true) {
			switch (*_ip) {
			case Op::Return: return;

			case Op::Push:
				advance();
				std::array<char, 8> x;
				for (int i = 0; i < 8; ++i) {
					x[i] = *_ip;
					advance();
				}
				push(std::bit_cast<double>(x));
				break;

			case Op::PushString: {
				advance();
				std::stringstream ss{};
				while (*_ip) {
					ss.put(static_cast<char>(*_ip));
					advance();
				}
				advance();
				push(ss.str());
				break;
			}

			case Op::Pop:
				pop();
				advance();
				break;

			case Op::NumericIn: {
				double res;
				std::cin >> res;
				push(res);
				advance();
				break;
			}

			case Op::In: {
				std::string res;
				std::cin >> res;
				push(res);
				advance();
				break;
			}

			case Op::AsciiIn: {
				char res;
				std::cin >> res;
				push(static_cast<double>(res));
				advance();
				break;
			}

			case Op::Out:
				std::visit([](auto const &val) { fmt::print("{}", val); }, pop());
				advance();
				break;

			case Op::AsciiOut:
				std::visit(
				    overloaded{
				        [](double const &&val) { fmt::print("{}", static_cast<char>(val)); },
				        [](std::string const &&) { throw std::runtime_error{"Ascii out on string"}; }},
				    pop());
				advance();
				break;

			case Op::Swap:
				swap();
				advance();
				break;

			case Op::Reverse:
				reverse();
				advance();
				break;

			case Op::Dup:
				dup();
				advance();
				break;

#define X(OpCode, Fn) \
	case OpCode: { \
		if (_stack.size() < 2) { \
			throw StackException{}; \
		} \
		NoriValue &a = peek(1); \
		NoriValue &b = peek(0); \
		std::visit( \
		    overloaded{ \
		        [&](double const &a_val, double const &b_val) -> void { a.emplace<double>((Fn)(a_val, b_val)); }, \
		        [](auto, auto) -> void { throw InvalidOperandException{}; }}, \
		    a, b); \
		pop(); \
		advance(); \
		break; \
	}

				XBINOPS
#undef X

			case Op::Root:
				push(root(pop()));
				advance();
				break;

			case Op::Ceil:
				push(ceil(pop()));
				advance();
				break;

			case Op::Floor:
				push(floor(pop()));
				advance();
				break;

			case Op::Rand:
				push(double_dis(rng));
				advance();
				break;

			case Op::BitRand:
				push(static_cast<double>(byte_dis(rng) % 2));
				advance();
				break;

			case Op::ByteRand:
				push(static_cast<double>(byte_dis(rng) % 256));
				advance();
				break;

			case Op::JumpBegin:
				load(0);
				_ip = _buffer;
				break;

			case Op::ForwardJumpFalse: {
				advance();
				std::uint8_t distance = *_ip;
				advance();
				if (!truthy(peek())) {
					if ((_ip - _buffer) + distance >= _buffer_size) {
						load(cur_pos() + distance);
						_ip = _buffer;
					} else {
						_ip += distance;
					}
				}
				break;
			}

			case Op::BackwardJumpTrue: {
				advance();
				std::uint8_t distance = *_ip;
				advance();
				if (truthy(peek())) {
					if ((_ip - _buffer) - distance < 0) {
						load(cur_pos() - distance);
						_ip = _buffer;
					} else {
						_ip -= distance;
					}
				}
				break;
			}

			default:
				std::stringstream s{};
				s << "Unhandled opcode " << static_cast<int>(*_ip) << " at " << cur_pos();
				throw std::runtime_error{std::string{s.str()}};
			}
		}
	}

  private:
	T &_stream;
	std::uint8_t *const _buffer;
	std::size_t const _buffer_size;
	std::size_t _buffer_offset;
	std::uint8_t const *_ip;
	std::deque<NoriValue> _stack;
	bool _reversed;

	// Randomness

	std::random_device device;
	std::mt19937 rng{device()};
	std::uniform_real_distribution<double> double_dis{
	    std::numeric_limits<double>::min(), std::numeric_limits<double>::max()};
	std::uniform_int_distribution<unsigned> byte_dis{
	    std::numeric_limits<unsigned>::min(), std::numeric_limits<unsigned>::max()};

	// Bytecode loading

	void advance() {
		++_ip;
		if (_ip >= _buffer + _buffer_size) {
			load(_buffer_offset + _buffer_size);
			_ip = _buffer;
		}
	}

	void load(std::size_t pos) {
		_stream.clear();
		_stream.seekg(pos, std::ios_base::beg);
		_stream.read(reinterpret_cast<char *>(_buffer), _buffer_size);
		_buffer_offset = pos;
	}

	std::size_t cur_pos() const { return _buffer_offset + (_ip - _buffer); }

	// Stack manipulation

	void push(NoriValue value) {
		if (_reversed) {
			_stack.emplace_front(value);
		} else {
			_stack.emplace_back(value);
		}
	}

	NoriValue pop() {
		if (_stack.size() == 0) {
			throw StackException{};
		}
		NoriValue ret_val;
		if (_reversed) {
			ret_val = _stack.front();
			_stack.pop_front();
		} else {
			ret_val = _stack.back();
			_stack.pop_back();
		}
		return ret_val;
	}

	NoriValue &peek() { return _reversed ? _stack.front() : _stack.back(); }
	NoriValue &peek(int i) { return _reversed ? _stack[i] : _stack[_stack.size() - 1 - i]; }

	void swap() {
		if (_stack.size() < 2) {
			throw StackException{};
		}
		if (_reversed) {
			auto &first = _stack[0];
			auto &second = _stack[1];
			std::swap(first, second);
		} else {
			auto &first = _stack[_stack.size() - 1];
			auto &second = _stack[_stack.size() - 2];
			std::swap(first, second);
		}
	}

	void reverse() { _reversed = !_reversed; }

	void dup() {
		if (_stack.size() == 0) {
			throw StackException{};
		}
		if (_reversed) {
			auto first = _stack.front();
			_stack.emplace_front(first);
		} else {
			auto first = _stack.back();
			_stack.emplace_back(first);
		}
	}
};

} // namespace nori::vm

#endif