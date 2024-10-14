#include <doctest.hpp>
#include <optional>
#include <structopt.hpp>

using doctest::test_suite;

struct Commands {
	struct New : structopt::sub_command {
		std::string projectName;
	};

	// sub commands
	New new_;
};

// NOLINTNEXTLINE
STRUCTOPT(Commands::New, projectName);
// NOLINTNEXTLINE
STRUCTOPT(Commands, new_);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse ristricted keywords" * test_suite("ristricted_arguments")) {
	auto arguments = structopt::app("test").parse<Commands>(
		std::vector<std::string>{ "./main", "new", "my_project" });
	REQUIRE(arguments.new_.projectName == "my_project");
}

struct MultiCommands {
	struct Using : structopt::sub_command {
		std::string thing;
	};

	// sub commands
	Using using_;

	struct Delete : structopt::sub_command {
		std::optional<bool> flag = false;
		std::string name;
	};

	Delete delete_;
};

// NOLINTNEXTLINE
STRUCTOPT(MultiCommands::Using, thing);
// NOLINTNEXTLINE
STRUCTOPT(MultiCommands::Delete, flag, name);
// NOLINTNEXTLINE
STRUCTOPT(MultiCommands, using_, delete_);

auto app = structopt::app("test");

// NOLINTNEXTLINE
TEST_CASE("structopt can parse ristricted keywords" * test_suite("ristricted_arguments")) {
	auto arguments =
		app.parse<MultiCommands>(std::vector<std::string>{ "./main", "using", "AThing" });

	REQUIRE(arguments.using_.thing == "AThing");
}

// NOLINTNEXTLINE
TEST_CASE("structopt can parse ristricted keywords" * test_suite("ristricted_arguments")) {
	auto arguments =
		app.parse<MultiCommands>(std::vector<std::string>{ "./main", "delete", "AThing" });
	REQUIRE(arguments.delete_.name == "AThing");

	arguments = app.parse<MultiCommands>(
		std::vector<std::string>{ "./main", "delete", "-flag", "OldThing" });
	// NOLINTNEXTLINE
	REQUIRE(arguments.delete_.flag == true);
	REQUIRE(arguments.delete_.name == "OldThing");
}

// NOLINTNEXTLINE
TEST_CASE("structopt can parse ristricted keywords" * test_suite("ristricted_arguments")) {
	auto arguments =
		app.parse<MultiCommands>(std::vector<std::string>{ "./main", "delete", "-f", "OldThing" });
	REQUIRE(arguments.delete_.flag == true);
	REQUIRE(arguments.delete_.name == "OldThing");
}

// NOLINTNEXTLINE
TEST_CASE("structopt can parse ristricted keywords" * test_suite("ristricted_arguments")) {
	auto failed = false;
	try {

		auto arguments =
			app.parse<MultiCommands>(std::vector<std::string>{ "./main", "delete", "-f" });
	} catch(...) {
		failed = true;
	}

	REQUIRE(failed == true);
}
