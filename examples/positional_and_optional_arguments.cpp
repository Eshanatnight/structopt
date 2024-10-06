#include "structopt.hpp"

struct Option {
	int input{ 0 };
	std::optional<bool> verbose = false;
};
// NOLINTNEXTLINE
STRUCTOPT(Option, input, verbose);

auto main(int argc, char* argv[]) -> int {

	try {
		auto options = structopt::app("my_app").parse<Option>(argc, argv);
		if(options.verbose == true) {
			std::cout << "The square of " << options.input << " is "
					  << (options.input * options.input) << "\n";
		} else {
			std::cout << options.input * options.input << "\n";
		}
	} catch(structopt::exception& e) {
		std::cout << e.what() << "\n";
		std::cout << e.help();
	}
}
