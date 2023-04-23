#pragma once
#ifndef TOKENITER_HPP
#define TOKENITER_HPP

#include <charconv>
#include <cstddef>
#include <sstream>
#include <string_view>

#include "token.hpp"

namespace nori::parse {

class Tokens {
	std::string_view const &_source;

  public:
	Tokens(std::string_view const &source) : _source(source) {}

	class iterator {
		using InnerIterator = std::string_view::const_iterator;

	  private:
		InnerIterator _end;
		InnerIterator _cur;
		bool _ended;
		Token _currentToken;

	  public:
		iterator(InnerIterator end, InnerIterator cur) : _end{end}, _cur{cur}, _ended{false} { ++*this; }

		iterator &operator++() {
			while (_cur != _end) {
				switch (*_cur) {

#define X(Name, Symbol) \
	case Symbol: \
		++_cur; \
		_currentToken = {.type = TokenType::Name}; \
		break;

					XSYMBOLS

#undef X

				case '0' ... '9': {
					auto const begin = _cur;
					while (_cur != _end && '0' <= *_cur && *_cur <= '9')
						++_cur;

					std::string_view const view{begin, _cur};
					float res;
					std::from_chars(view.data(), view.data() + view.size(), res);
					_currentToken = {.type = TokenType::Value, .value = res};
					break;
				}
				case '|':
				case '"':
				case '\'': {
					char const quote_char = *_cur;
					++_cur;
					auto const begin = _cur;
					while (_cur != _end && *_cur != quote_char)
						++_cur;

					_currentToken = {.type = TokenType::Value, .value = std::string{begin, _cur}};
					++_cur;
					break;
				}
				default: ++_cur; continue;
				}
				return *this;
			}
			_ended = true;
			return *this;
		}
		iterator operator++(int) {
			iterator retval{*this};
			++*this;
			return retval;
		};
		bool operator==(iterator other) const { return this->_cur == other._cur && this->_ended == other._ended; }
		bool operator!=(iterator other) const { return !(*this == other); }
		Token operator*() const { return _currentToken; }

		// iterator traits
		using difference_type = std::ptrdiff_t;
		using value_type = Token;
		using pointer = Token const *;
		using reference = Token const &;
		using iterator_category = std::forward_iterator_tag;
	};

	iterator begin() { return iterator(std::end(_source), std::begin(_source)); }
	iterator end() { return iterator(std::end(_source), std::end(_source)); }
};

} // namespace nori::parse

#endif