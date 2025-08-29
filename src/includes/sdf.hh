#ifndef __SDF__
#define __SDF__

#include <array>
#include <vector>
#include <functional>
#include <optional>
#include <unordered_map>
#include <iostream>
#include <fstream>

namespace Sdf {
  enum class State : unsigned short {
    START = 0U,
    IDENTIFIER, NUMBER,
    STRING_START, STRING_BODY, STRING_END,
    CHAR_START, CHAR_BODY, CHAR_END,
    LPAREN, RPAREN, LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    AT, DOLLAR, BANG,
    COMMA, COLON,
    BOOLEAN, TRUE, FALSE,
    CHAR, STRING, INT, FLOAT,
    BODY, HEADER, SCHEME,
    ERROR, NUM_STATES
  };

  class Token {
    public:
    const Sdf::State type;
    const std::string_view lexum;

    Token(Sdf::State type, std::string_view lexum) : type(type), lexum(lexum) { };
    ~Token() = default;
  };

  template <typename T>
  class Compiler {
    public:
    const T data;

    template <typename F>
    Compiler<F> operator|(const std::function<F(T)>& f) {
      return Compiler<F>(f(this->data));
    }

    Compiler(T data) : data(data) { };
    ~Compiler() = default;
  };

  extern const std::function<std::vector<Sdf::Token>(const std::string_view)> lexical;
}

#endif
