#include "./includes/sdf-mbf.hh"

int main(int argc, char** argv) {
  SdfMbf::Compiler<std::string> initial("{ !$header: { }, !$body: { foo: \"bar\" } }");

  auto final = initial | Lexer::lexical; // fix later

  return 0;
}

constexpr SdfMbf::LexTable Lexer::buildTable() {
  SdfMbf::LexTable table = { };

  for (size_t i = 0; i < (size_t) SdfMbf::State::NUM_STATES; i++) {
    for (size_t j = 0; j < 128; j++) {
      table[i][j] = SdfMbf::State::ERROR;
    }
  }

  for (size_t i = 'A'; i <= 'Z'; i++) {
    table[(size_t) SdfMbf::State::START][(size_t) i] = SdfMbf::State::IDENTIFIER;
    table[(size_t) SdfMbf::State::IDENTIFIER][(size_t) i] = SdfMbf::State::IDENTIFIER;
  }

  for (size_t i = 'a'; i <= 'z'; i++) {
    table[(size_t) SdfMbf::State::START][(size_t) i] = SdfMbf::State::IDENTIFIER;
    table[(size_t) SdfMbf::State::IDENTIFIER][(size_t) i] = SdfMbf::State::IDENTIFIER;
  }

  for (size_t i = '0'; i <= '9'; i++) {
    table[(size_t) SdfMbf::State::START][(size_t) i] = SdfMbf::State::NUMBER;
    table[(size_t) SdfMbf::State::NUMBER][(size_t) i] = SdfMbf::State::NUMBER;
    table[(size_t) SdfMbf::State::IDENTIFIER][(size_t) i] = SdfMbf::State::IDENTIFIER;
  }

  table[(size_t) SdfMbf::State::START][(size_t) '('] = SdfMbf::State::LPAREN;
  table[(size_t) SdfMbf::State::START][(size_t) ')'] = SdfMbf::State::RPAREN;
  table[(size_t) SdfMbf::State::START][(size_t) '{'] = SdfMbf::State::LBRACE;
  table[(size_t) SdfMbf::State::START][(size_t) '}'] = SdfMbf::State::RBRACE;
  table[(size_t) SdfMbf::State::START][(size_t) '['] = SdfMbf::State::LBRACKET;
  table[(size_t) SdfMbf::State::START][(size_t) ']'] = SdfMbf::State::RBRACKET;

  table[(size_t) SdfMbf::State::START][(size_t) '@'] = SdfMbf::State::AT;
  table[(size_t) SdfMbf::State::START][(size_t) '$'] = SdfMbf::State::DOLLAR;
  table[(size_t) SdfMbf::State::START][(size_t) '!'] = SdfMbf::State::BANG;

  table[(size_t) SdfMbf::State::START][(size_t) ':'] = SdfMbf::State::COLON;
  table[(size_t) SdfMbf::State::START][(size_t) ','] = SdfMbf::State::COMMA;

  return table;
}

SdfMbf::Token Lexer::transition() {
  const size_t initial = i;
  // SdfMbf::State current = Lexer::table[(size_t) last][(size_t) source[i]];

  /*
  if (current != SdfMbf::State::ERROR && source.size() > i + 1) {
    i++;
    return transition(c);
  }
  */

  return SdfMbf::Token(last, source.substr(initial, i - initial));
}

const std::function<std::vector<SdfMbf::Token>(std::string&)> Lexer::lexical = [](const std::string& source) {
  std::vector<SdfMbf::Token> tokens = { }; // maybe add this to Lexer class ?
  Lexer lexer(source);

  for (size_t i = 0; i < source.size(); i++) {
    switch (source[i]) {
      default:
      tokens.push_back(lexer.transition());
      break;
    }
  }

  return tokens;
};
