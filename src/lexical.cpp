#include "./includes/sdf.hh"

namespace {
  using LexTable = std::array<std::array<Sdf::State, 128>, (size_t) Sdf::State::NUM_STATES>;
  using AccTable = std::array<bool, (size_t) Sdf::State::NUM_STATES>;

  const std::unordered_map<std::string_view, Sdf::State> keywords = {
    {"Boolean", Sdf::State::BOOLEAN}, {"true", Sdf::State::TRUE}, {"false", Sdf::State::FALSE},
    {"Char", Sdf::State::CHAR}, {"String,", Sdf::State::STRING}, {"Int", Sdf::State::INT},
    {"Float", Sdf::State::FLOAT}, {"body", Sdf::State::BODY}, {"header", Sdf::State::HEADER},
    {"scheme", Sdf::State::SCHEME}
  };

  inline constexpr LexTable buildTransitions() {
    LexTable table = { };

    for (size_t i = 0; i < (size_t) Sdf::State::NUM_STATES; i++) {
      for (size_t j = 0; j < 128; j++) {
        table[i][j] = Sdf::State::ERROR;
      }
    }

    for (size_t i = (size_t) 'A'; i <= (size_t) 'Z'; i++) {
      table[(size_t) Sdf::State::START][i] = Sdf::State::IDENTIFIER;
      table[(size_t) Sdf::State::IDENTIFIER][i] = Sdf::State::IDENTIFIER;
    }

    for (size_t i = (size_t) 'a'; i <= (size_t) 'z'; i++) {
      table[(size_t) Sdf::State::START][i] = Sdf::State::IDENTIFIER;
      table[(size_t) Sdf::State::IDENTIFIER][i] = Sdf::State::IDENTIFIER;
    }

    for (size_t i = (size_t) '0'; i <= (size_t) '9'; i++) {
      table[(size_t) Sdf::State::START][i] = Sdf::State::NUMBER;
      table[(size_t) Sdf::State::NUMBER][i] = Sdf::State::NUMBER;
      table[(size_t) Sdf::State::IDENTIFIER][i] = Sdf::State::IDENTIFIER;
    }

    table[(size_t) Sdf::State::START][(size_t) ' '] = Sdf::State::START;
    table[(size_t) Sdf::State::START][(size_t) '\t'] = Sdf::State::START;
    table[(size_t) Sdf::State::START][(size_t) '\n'] = Sdf::State::START;

    table[(size_t) Sdf::State::START][(size_t) '('] = Sdf::State::LPAREN;
    table[(size_t) Sdf::State::START][(size_t) ')'] = Sdf::State::RPAREN;
    table[(size_t) Sdf::State::START][(size_t) '{'] = Sdf::State::LBRACE;
    table[(size_t) Sdf::State::START][(size_t) '}'] = Sdf::State::RBRACE;
    table[(size_t) Sdf::State::START][(size_t) '['] = Sdf::State::LBRACKET;
    table[(size_t) Sdf::State::START][(size_t) ']'] = Sdf::State::RBRACKET;
  
    table[(size_t) Sdf::State::START][(size_t) '@'] = Sdf::State::AT;
    table[(size_t) Sdf::State::START][(size_t) '$'] = Sdf::State::DOLLAR;
    table[(size_t) Sdf::State::START][(size_t) '!'] = Sdf::State::BANG;

    table[(size_t) Sdf::State::START][(size_t) ':'] = Sdf::State::COLON;
    table[(size_t) Sdf::State::START][(size_t) ','] = Sdf::State::COMMA;

    return table;
  }

  inline constexpr AccTable buildAccepting() {
    AccTable accepting = { };

    for (size_t i = 0; i < (size_t) Sdf::State::NUM_STATES; i++) {
      accepting[i] = false;
    }

    accepting[(size_t) Sdf::State::IDENTIFIER] = true;
    accepting[(size_t) Sdf::State::NUMBER] = true;

    accepting[(size_t) Sdf::State::LPAREN] = true;
    accepting[(size_t) Sdf::State::RPAREN] = true;
    accepting[(size_t) Sdf::State::LBRACE] = true;
    accepting[(size_t) Sdf::State::RBRACE] = true;
    accepting[(size_t) Sdf::State::LBRACKET] = true;
    accepting[(size_t) Sdf::State::RBRACKET] = true;

    accepting[(size_t) Sdf::State::AT] = true;
    accepting[(size_t) Sdf::State::DOLLAR] = true;
    accepting[(size_t) Sdf::State::BANG] = true;
    accepting[(size_t) Sdf::State::COMMA] = true;
    accepting[(size_t) Sdf::State::COLON] = true;

    accepting[(size_t) Sdf::State::BOOLEAN] = true;
    accepting[(size_t) Sdf::State::TRUE] = true;
    accepting[(size_t) Sdf::State::FALSE] = true;

    return accepting;
  }
}

const std::function<std::vector<Sdf::Token>(const std::string_view)> Sdf::lexical = [](const std::string_view source){
  const LexTable table = buildTransitions();
  const AccTable accepting = buildAccepting();
  Sdf::State last = Sdf::State::START;
  std::optional<Sdf::State> lastAccepting = std::nullopt;
  std::vector<Sdf::Token> tokens = { };

  for (size_t i = 0, initial = i; i < source.size(); i++) {
    last = table[(size_t) last][(size_t) source[i]];
    if (accepting[(size_t) last]) lastAccepting = last;

    if (last == Sdf::State::ERROR && lastAccepting.has_value()) {
      tokens.push_back(Sdf::Token(*lastAccepting, source.substr(initial, i - initial)));
      initial = i;
    };
  }

  return tokens;
};
