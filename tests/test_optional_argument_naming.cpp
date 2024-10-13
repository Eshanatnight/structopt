#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct OptionalArgumentLongName {
	std::optional<std::string> input_file = "";
};

// NOLINTNEXTLINE
STRUCTOPT(OptionalArgumentLongName, input_file);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single optional argument with '_' separated name" *
		  test_suite("single_optional")) {
	{
		auto arguments = structopt::app("test").parse<OptionalArgumentLongName>(
			std::vector<std::string>{ "./main", "--input_file", "foo.csv" });
		REQUIRE(arguments.input_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<OptionalArgumentLongName>(
			std::vector<std::string>{ "./main", "-input_file", "foo.csv" });
		REQUIRE(arguments.input_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<OptionalArgumentLongName>(
			std::vector<std::string>{ "./main", "-i", "foo.csv" });
		REQUIRE(arguments.input_file == "foo.csv");
	}
	{
		auto arguments = structopt::app("test").parse<OptionalArgumentLongName>(
			std::vector<std::string>{ "./main" });
		REQUIRE(arguments.input_file == "");
	}
	{
		auto arguments = structopt::app("test").parse<OptionalArgumentLongName>(
			std::vector<std::string>{ "./main", "--input-file", "bar.csv" });
		REQUIRE(arguments.input_file == "bar.csv");
	}
}
