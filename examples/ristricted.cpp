#include "structopt.hpp"

#include <iostream>
#include <string>

struct Cli {
	struct New : structopt::sub_command {
		std::string projectName;
	};

	// sub commands
	New new_;
};

// NOLINTNEXTLINE
STRUCTOPT(Cli::New, projectName);
// NOLINTNEXTLINE
STRUCTOPT(Cli, new_);

auto main(int argc, char** argv) -> int {
	try {
		const auto cli = structopt::app("main").parse<Cli>(argc, argv);

		// Print out parsed arguments:
		std::cout << "New project name: " << cli.new_.projectName << "\n";

	} catch(const structopt::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.help();
	}
	return 0;
}
