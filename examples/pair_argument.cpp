#include "structopt.hpp"

struct Options {
	std::pair<std::string, std::string> name;
};
// NOLINTNEXTLINE
STRUCTOPT(Options, name);

auto main(int argc, char* argv[]) -> int {

	try {
		auto options = structopt::app("my_app").parse<Options>(argc, argv);

		std::cout << "First name : " << options.name.first << "\n";
		std::cout << "Last name  : " << options.name.second << "\n";
	} catch(structopt::exception& e) {
		std::cout << e.what() << "\n";
		std::cout << e.help();
	}
}
