#include "structopt.hpp"

struct Options {
	// Optional argument
	// -f, --fixed_point, --fixed-point <fixed_point...>
	std::optional<std::array<float, 3>> fixed_point;
};
// NOLINTNEXTLINE
STRUCTOPT(Options, fixed_point);

auto main(int argc, char* argv[]) -> int {
	try {
		auto options = structopt::app("my_app").parse<Options>(argc, argv);

		if(options.fixed_point.has_value()) {
			std::cout << "Point: {" << options.fixed_point.value()[0] << ", "
					  << options.fixed_point.value()[1] << ", " << options.fixed_point.value()[2]
					  << "}\n";
		}
	} catch(structopt::exception& e) {
		std::cout << e.what() << "\n";
		std::cout << e.help();
	}
}
