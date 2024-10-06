#include "structopt.hpp"

struct IntegerLiterals {
	std::vector<int> numbers;
};
// NOLINTNEXTLINE
STRUCTOPT(IntegerLiterals, numbers);

auto main(int argc, char* argv[]) -> int {
	try {
		auto options = structopt::app("my_app").parse<IntegerLiterals>(argc, argv);

		for(auto& n: options.numbers) std::cout << n << "\n";
	} catch(structopt::exception& e) {
		std::cout << e.what() << "\n";
		std::cout << e.help();
	}
}
