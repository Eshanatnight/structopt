#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct TupleOptionalArgument {
	std::optional<std::tuple<int, float, std::string>> value;
};

// NOLINTNEXTLINE
STRUCTOPT(TupleOptionalArgument, value);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional tuple argument" * test_suite("tuple_positional")) {
	{
		auto arguments = structopt::app("test").parse<TupleOptionalArgument>(
			std::vector<std::string>{ "./main" });
		REQUIRE(arguments.value.has_value() == false);
	}
	{
		auto arguments = structopt::app("test").parse<TupleOptionalArgument>(
			std::vector<std::string>{ "./main", "-v", "1", "3.14", "Hello World" });
		REQUIRE(arguments.value.has_value() == true);
		// NOLINTNEXTLINE
		REQUIRE(std::get<0>(arguments.value.value()) == 1);
		// NOLINTNEXTLINE
		REQUIRE(std::get<1>(arguments.value.value()) == 3.14F);
		// NOLINTNEXTLINE
		REQUIRE(std::get<2>(arguments.value.value()) == "Hello World");
	}
	{
		auto arguments = structopt::app("test").parse<TupleOptionalArgument>(
			std::vector<std::string>{ "./main", "--value", "1", "3.14", "Hello World" });
		REQUIRE(arguments.value.has_value() == true);
		// NOLINTNEXTLINE
		REQUIRE(std::get<0>(arguments.value.value()) == 1);
		// NOLINTNEXTLINE
		REQUIRE(std::get<1>(arguments.value.value()) == 3.14f);
		// NOLINTNEXTLINE
		REQUIRE(std::get<2>(arguments.value.value()) == "Hello World");
	}
}

struct MultipleTupleOptionalArguments {
	std::optional<std::tuple<int, float, std::string>> first;
	std::optional<std::tuple<char, bool>> second;
};

// NOLINTNEXTLINE
STRUCTOPT(MultipleTupleOptionalArguments, first, second);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional tuple argument" * test_suite("tuple_positional")) {
	{
		auto arguments = structopt::app("test").parse<MultipleTupleOptionalArguments>(
			std::vector<std::string>{ "./main" });
		REQUIRE(arguments.first.has_value() == false);
		REQUIRE(arguments.second.has_value() == false);
	}
	{
		auto arguments = structopt::app("test").parse<MultipleTupleOptionalArguments>(
			std::vector<std::string>{ "./main", "--first", "1", "3.14", "Hello World" });
		REQUIRE(arguments.first.has_value() == true);
		// NOLINTNEXTLINE
		REQUIRE(std::get<0>(arguments.first.value()) == 1);
		// NOLINTNEXTLINE
		REQUIRE(std::get<1>(arguments.first.value()) == 3.14F);
		// NOLINTNEXTLINE
		REQUIRE(std::get<2>(arguments.first.value()) == "Hello World");
		REQUIRE(arguments.second.has_value() == false);
	}
	{
		auto arguments = structopt::app("test").parse<MultipleTupleOptionalArguments>(
			std::vector<std::string>{ "./main", "--second", "a", "false" });
		REQUIRE(arguments.first.has_value() == false);
		REQUIRE(arguments.second.has_value() == true);
		// NOLINTNEXTLINE
		REQUIRE(std::get<0>(arguments.second.value()) == 'a');
		// NOLINTNEXTLINE
		REQUIRE(std::get<1>(arguments.second.value()) == false);
	}
	{
		auto arguments =
			structopt::app("test").parse<MultipleTupleOptionalArguments>(std::vector<std::string>{
				"./main", "--first", "1", "3.14", "Hello World", "--second", "c", "true" });
		REQUIRE(arguments.first.has_value() == true);
		REQUIRE(arguments.second.has_value() == true);
		// NOLINTNEXTLINE
		REQUIRE(std::get<0>(arguments.first.value()) == 1);
		// NOLINTNEXTLINE
		REQUIRE(std::get<1>(arguments.first.value()) == 3.14F);
		// NOLINTNEXTLINE
		REQUIRE(std::get<2>(arguments.first.value()) == "Hello World");
		// NOLINTNEXTLINE
		REQUIRE(std::get<0>(arguments.second.value()) == 'c');
		// NOLINTNEXTLINE
		REQUIRE(std::get<1>(arguments.second.value()) == true);
	}
}
