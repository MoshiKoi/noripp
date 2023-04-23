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

#include "../common.hpp"
#include "op.hpp"

namespace nori::vm {

static_assert(std::numeric_limits<float>::is_iec559, "Floating point format must be IEEE binary32");
static_assert(CHAR_BIT == 8, "Bytecode requires chars to be 8 bits");

class InvalidOperandException {};

#define XBINOPS \
	X(add, +) \
	X(sub, -) \
	X(div, /) \
	X(mul, *)

#define X(Name, _) NoriValue Name(NoriValue const &, NoriValue const &);

XBINOPS

NoriValue pow(NoriValue const &, NoriValue const &);
NoriValue mod(NoriValue const &, NoriValue const &);
NoriValue floor(NoriValue const &);
NoriValue ceil(NoriValue const &);
NoriValue root(NoriValue const &);

#undef X

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
				std::array<char, 4> x;
				x[0] = *_ip;
				advance();
				x[1] = *_ip;
				advance();
				x[2] = *_ip;
				advance();
				x[3] = *_ip;
				advance();
				push(std::bit_cast<float>(x));
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

			case Op::In: {
				std::string res;
				std::cin >> res;
				push(res);
				advance();
				break;
			}

			case Op::Out:
				std::visit([](auto const &val) { std::cout << val; }, pop());
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

			case Op::Add:
				push(add(pop(), pop()));
				advance();
				break;

			case Op::Sub: {
				auto x = pop();
				auto y = pop();
				push(sub(x, y));
				advance();
				break;
			}

			case Op::Div: {
				auto x = pop();
				auto y = pop();
				push(div(x, y));
				advance();
				break;
			}

			case Op::Mul:
				push(mul(pop(), pop()));
				advance();
				break;

			case Op::Pow: {
				auto x = pop();
				auto y = pop();
				push(pow(x, y));
				advance();
				break;
			}

			case Op::Root:
				push(root(pop()));
				advance();
				break;

			case Op::Mod: {
				auto x = pop();
				auto y = pop();
				push(mod(x, y));
				advance();
				break;
			}

			case Op::Ceil:
				push(ceil(pop()));
				advance();
				break;

			case Op::Floor:
				push(floor(pop()));
				advance();
				break;

			case Op::Rand:
				push(float_dis(rng));
				advance();
				break;

			case Op::BitRand:
				push(static_cast<float>(byte_dis(rng) % 2));
				advance();
				break;

			case Op::ByteRand:
				push(static_cast<float>(byte_dis(rng) % 256));
				advance();
				break;

			case Op::JumpBegin:
				load(0);
				_ip = _buffer;
				break;

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
	std::uint8_t *_ip;
	std::deque<NoriValue> _stack;
	bool _reversed;

	// Randomness

	std::random_device device;
	std::mt19937 rng{device()};
	std::uniform_real_distribution<float> float_dis{
	    std::numeric_limits<float>::min(), std::numeric_limits<float>::max()};
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
		_stream.seekg(pos, std::ios_base::beg);
		_stream.read(reinterpret_cast<char *>(_buffer), _buffer_size);
		_buffer_offset = pos;
	}

	std::size_t cur_pos() { return _buffer_offset + (_ip - _buffer); }

	// Stack manipulation

	void push(NoriValue value) {
		if (_reversed) {
			_stack.emplace_front(value);
		} else {
			_stack.emplace_back(value);
		}
	}

	NoriValue pop() {
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

	void swap() {
		if (_reversed) {
			auto first = _stack.front();
			_stack.pop_front();
			_stack.emplace(_stack.begin() + 1, first);
		} else {
			auto first = _stack.back();
			_stack.pop_back();
			_stack.emplace(_stack.end() - 1, first);
		}
	}

	void reverse() { _reversed = !_reversed; }

	void dup() {
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