#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct Command {
	struct SubCommand1 : structopt::sub_command {
		int bar{ 0 };
		double value{ 0.0 };
		std::optional<bool> verbose;
	};
	SubCommand1 foo;

	struct SubCommand2 : structopt::sub_command {
		std::optional<bool> global;
	};
	SubCommand2 config;
};
// NOLINTNEXTLINE
STRUCTOPT(Command::SubCommand1, bar, value, verbose);
// NOLINTNEXTLINE
STRUCTOPT(Command::SubCommand2, global);
// NOLINTNEXTLINE
STRUCTOPT(Command, foo, config);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse multiple nested struct arguments" * test_suite("nested_struct")) {
	{
		auto arguments = structopt::app("test").parse<Command>(
			std::vector<std::string>{ "./main", "foo", "15", "3.14", "--verbose", "true" });
		REQUIRE(arguments.foo.has_value() == true);
		REQUIRE(arguments.foo.bar == 15);
		REQUIRE(arguments.foo.value == 3.14);
		// NOLINTNEXTLINE
		REQUIRE(arguments.foo.verbose.value() == true);
		REQUIRE(!arguments.config.has_value());
	}
	{
		auto arguments = structopt::app("test").parse<Command>(
			std::vector<std::string>{ "./main", "foo", "-v", "true", "15", "3.14" });
		REQUIRE(arguments.foo.has_value() == true);
		REQUIRE(arguments.foo.bar == 15);
		REQUIRE(arguments.foo.value == 3.14);
		// NOLINTNEXTLINE
		REQUIRE(arguments.foo.verbose.value() == true);
		REQUIRE(!arguments.config.has_value());
	}
	{
		auto arguments = structopt::app("test").parse<Command>(
			std::vector<std::string>{ "./main", "config", "--global", "true" });
		REQUIRE(arguments.foo.has_value() == false);
		REQUIRE(arguments.foo.bar == 0);
		REQUIRE(arguments.foo.value == 0.0);
		REQUIRE(arguments.foo.verbose.has_value() == false);
		REQUIRE(arguments.config.has_value() == true);
		REQUIRE(arguments.config.global == true);
	}
	{
		auto arguments = structopt::app("test").parse<Command>(
			std::vector<std::string>{ "./main", "config", "-g", "false" });
		REQUIRE(arguments.foo.has_value() == false);
		REQUIRE(arguments.foo.bar == 0);
		REQUIRE(arguments.foo.value == 0.0);
		REQUIRE(arguments.foo.verbose.has_value() == false);
		REQUIRE(arguments.config.has_value() == true);
		REQUIRE(arguments.config.global == false);
	}
	{
		bool exception_thrown = false;
		try {
			auto arguments = structopt::app("test").parse<Command>(std::vector<std::string>{
				"./main", "foo", "15", "3.14", "--verbose", "true", "config", "-g", "false" });
		} catch(structopt::exception&) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown);
	}
	{
		bool exception_thrown = false;
		try {
			auto arguments = structopt::app("test").parse<Command>(std::vector<std::string>{
				"./main", "config", "-g", "false", "foo", "15", "3.14", "--verbose", "true" });
		} catch(structopt::exception&) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown);
	}
}

struct Git {
	// Subcommand: git config
	struct Config : structopt::sub_command {
		std::optional<bool> global = false;
		std::optional<bool> local  = true;
		std::array<std::string, 2> name_value_pair{};
	};
	Config config;

	// Subcommand: git init
	struct Init : structopt::sub_command {
		std::string name;
	};
	Init init;
};

// NOLINTNEXTLINE
STRUCTOPT(Git::Config, global, local, name_value_pair);
// NOLINTNEXTLINE
STRUCTOPT(Git::Init, name);
// NOLINTNEXTLINE
STRUCTOPT(Git, config, init);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse multiple nested struct arguments - Git example" *
		  test_suite("nested_struct")) {
	{
		auto arguments = structopt::app("test").parse<Git>(
			std::vector<std::string>{ "./main", "config", "user.name", "Foobar" });
		REQUIRE(arguments.config.global == false);
		REQUIRE(arguments.config.local == true);
		REQUIRE(arguments.config.name_value_pair ==
				std::array<std::string, 2>{ "user.name", "Foobar" });
		REQUIRE(arguments.init.name == "");
	}
	{
		auto arguments = structopt::app("test").parse<Git>(
			std::vector<std::string>{ "./main", "config", "--global", "user.name", "Foobar" });
		REQUIRE(arguments.config.global == true);
		REQUIRE(arguments.config.name_value_pair ==
				std::array<std::string, 2>{ "user.name", "Foobar" });
		REQUIRE(arguments.init.name == "");
	}
	{
		auto arguments = structopt::app("test").parse<Git>(
			std::vector<std::string>{ "./main", "init", "my_repo" });
		REQUIRE(arguments.config.global == false);
		REQUIRE(arguments.config.name_value_pair == std::array<std::string, 2>{});
		REQUIRE(arguments.init.name == "my_repo");
	}
}
