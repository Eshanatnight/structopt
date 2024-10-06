#include "structopt.hpp"

struct Options {
	// positional arguments
	std::string input_file;
	std::string output_file;

	// optional arguments
	std::optional<std::string> bind_address;

	// remaining arguments
	std::vector<std::string> files;
};
// NOLINTNEXTLINE
STRUCTOPT(Options, input_file, output_file, bind_address, files);

auto main(int argc, char* argv[]) -> int {

	try {
		const std::string& custom_help =
			"Usage: ./my_app input_file output_file [--bind-address BIND_ADDRESS] [files...]\n";
		auto options = structopt::app("my_app", "1.0.3", custom_help).parse<Options>(argc, argv);
	} catch(structopt::exception& e) {
		std::cout << e.what() << "\n";
		std::cout << e.help();
	}
}
