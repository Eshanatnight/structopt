#include "structopt.hpp"

struct FloatLiterals {
	std::vector<float> numbers;
};
// NOLINTNEXTLINE
STRUCTOPT(FloatLiterals, numbers);

auto main(int argc, char* argv[]) -> int {
	try {
		auto options = structopt::app("my_app").parse<FloatLiterals>(argc, argv);

		for(auto& n: options.numbers) std::cout << n << "\n";
	} catch(structopt::exception& e) {
		std::cout << e.what() << "\n";
		std::cout << e.help();
	}
}
