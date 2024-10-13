#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct TupleArgument {
	std::tuple<int, float, std::string> value;
};

// NOLINTNEXTLINE
STRUCTOPT(TupleArgument, value);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional tuple argument" * test_suite("tuple_positional")) {
	{
		auto arguments = structopt::app("test").parse<TupleArgument>(
			std::vector<std::string>{ "./main", "1", "3.14", "Hello World" });
		REQUIRE(std::get<0>(arguments.value) == 1);
		REQUIRE(std::get<1>(arguments.value) == 3.14F);
		REQUIRE(std::get<2>(arguments.value) == "Hello World");
	}
}

struct MultipleTupleArguments {
	std::tuple<int, float, std::string> first;
	std::tuple<char, bool> second;
};

// NOLINTNEXTLINE
STRUCTOPT(MultipleTupleArguments, first, second);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional tuple argument" * test_suite("tuple_positional")) {
	{
		auto arguments = structopt::app("test").parse<MultipleTupleArguments>(
			std::vector<std::string>{ "./main", "1", "3.14", "Hello World", "c", "true" });
		REQUIRE(std::get<0>(arguments.first) == 1);
		REQUIRE(std::get<1>(arguments.first) == 3.14F);
		REQUIRE(std::get<2>(arguments.first) == "Hello World");
		REQUIRE(std::get<0>(arguments.second) == 'c');
		REQUIRE(std::get<1>(arguments.second) == true);
	}
}
