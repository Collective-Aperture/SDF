#include "./includes/sdf.hh"

int main(int argc, char** argv) {
  Sdf::Compiler<std::string_view> initial("{ !$header: { }, !$body: { \"foo: bar\" } }");

  const auto final = initial | Sdf::lexical;

  // testing cases
  [](const Sdf::Compiler<std::vector<Sdf::Token>>& final) -> void {
    size_t i = 1;
    for (const auto& it : final.data) {
      std::cout << "{ Token-Number: " << i << ", Type: " << (size_t) it.type << ", Lexum: \"" << it.lexum << "\" }\n";
      i++;
    }
    std::cout << std::endl;
  }(final);

  return 0;
}
