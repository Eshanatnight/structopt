#include "structopt.hpp"

struct FloatLiterals {
  std::vector<float> numbers;
};
STRUCTOPT(FloatLiterals, numbers);

int main(int argc, char *argv[]) {
  try {
    auto options = structopt::app("my_app").parse<FloatLiterals>(argc, argv);

    for (auto &n : options.numbers)
      std::cout << n << "\n";
  } catch (structopt::exception &e) {
    std::cout << e.what() << "\n";
    std::cout << e.help();
  }
}
