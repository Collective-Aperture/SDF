#ifndef __SDF_MBF__
#define __SDF_MBF__

#include <array>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>

namespace SdfMbf {
  enum class State : unsigned short {
    START = 0U,
    IDENTIFIER, NUMBER,
    LPAREN, RPAREN, LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    AT, DOLLAR, BANG,
    COMMA, COLON,
    BOOLEAN, TRUE, FALSE, 
    ERROR, NUM_STATES
  };

  struct Token {
    public:
    const SdfMbf::State type;
    const std::string_view lexum;

    Token(const SdfMbf::State type, const std::string_view lexum) : type(type), lexum(lexum) { };
  };

  template <typename T>
  class Compiler {
    private:
    const T source;

    public:
    template <typename F>
    Compiler<F> operator|(const std::function<F(T)> f) {
      return Compiler<F>(f(this->source));
    }

    Compiler<T>(const T source) : source(source) { };
    ~Compiler<T>() = default;
  };

  using LexTable = std::array<std::array<SdfMbf::State, 128>, (size_t) SdfMbf::State::NUM_STATES>;

  // Compiler Functions && Classes
  class Lexer {
    private:
    static constexpr LexTable buildTable();
    SdfMbf::Token transition();

    const std::string_view source;
    const LexTable table = buildTable();
    SdfMbf::State last = SdfMbf::State::START;
    size_t i = 0;

    public:
    static const std::function<std::vector<SdfMbf::Token>(std::string&)> lexical;

    Lexer(const std::string& source) : source(source) { };
    ~Lexer() = default;
  };

} // namespace SdfMbf

using Lexer = SdfMbf::Lexer;

#endif
