#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct SingleOptionalBoolArgument {
	std::optional<bool> verbose;
};

// NOLINTNEXTLINE
STRUCTOPT(SingleOptionalBoolArgument, verbose);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single optional argument" * test_suite("single_optional")) {
	{
		bool exception_thrown = false;
		try {
			[[maybe_unused]] auto ignore = structopt::app("test").parse<SingleOptionalBoolArgument>(
				std::vector<std::string>{ "./main", "--verbose" });
		} catch(structopt::exception&) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown == true);
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalBoolArgument>(
			std::vector<std::string>{ "./main", "--verbose", "true" });
		REQUIRE(arguments.verbose == true);
	}
	{
		bool exception_thrown = false;
		try {
			[[maybe_unused]] auto ignore = structopt::app("test").parse<SingleOptionalBoolArgument>(
				std::vector<std::string>{ "./main", "--verbose", "trueee" });
		} catch(structopt::exception&) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown == true);
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalBoolArgument>(
			std::vector<std::string>{ "./main", "-v", "false" });
		REQUIRE(arguments.verbose == false);
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalBoolArgument>(
			std::vector<std::string>{ "./main" });
		REQUIRE(arguments.verbose.has_value() == false);
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalBoolArgument>(
			std::vector<std::string>{ "./main", "-v=false" });
		REQUIRE(arguments.verbose.has_value() == true);
		// NOLINTNEXTLINE
		REQUIRE(arguments.verbose.value() == false);
	}
}

struct SingleOptionalStringArgument {
	std::optional<std::string> bind_address;
	std::string config_file;
};

// NOLINTNEXTLINE
STRUCTOPT(SingleOptionalStringArgument, bind_address, config_file);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single optional argument with underscore-separated long name " *
		  test_suite("single_optional")) {
	{
		bool exception_thrown = false;
		try {
			[[maybe_unused]] auto ignore =
				structopt::app("test").parse<SingleOptionalStringArgument>(
					std::vector<std::string>{ "./main", "--bind_address" });
		} catch(structopt::exception&) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown == true);
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "-b", "localhost", "foo.csv" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "-b:192.168.7.1", "foo.csv" });
		REQUIRE(arguments.bind_address == "192.168.7.1");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "-b=192.168.7.1", "foo.csv" });
		REQUIRE(arguments.bind_address == "192.168.7.1");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "-bind-address", "localhost", "foo.csv" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "-bind_address", "localhost", "foo.csv" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "--bind-address", "192.168.153.47", "foo.csv" });
		REQUIRE(arguments.bind_address == "192.168.153.47");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "--bind_address", "192.168.153.47", "foo.csv" });
		REQUIRE(arguments.bind_address == "192.168.153.47");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "foo.csv", "-b", "localhost" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "foo.csv", "--bind-address", "localhost" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "foo.csv", "--bind_address", "localhost" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "foo.csv", "-bind-address", "localhost" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "foo.csv", "-bind_address", "localhost" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<SingleOptionalStringArgument>(
			std::vector<std::string>{ "./main", "foo.csv", "-bind_address", "localhost" });
		REQUIRE(arguments.bind_address == "localhost");
		REQUIRE(arguments.config_file == "foo.csv");
	}
}
