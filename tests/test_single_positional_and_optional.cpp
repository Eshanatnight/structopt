#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct PositionalAndOptionalArgument {
	int foo = 0;
	std::optional<bool> bar;
};

// NOLINTNEXTLINE
STRUCTOPT(PositionalAndOptionalArgument, foo, bar);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional and optional arguments" *
		  test_suite("single_positional_and_optional")) {
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArgument>(
			std::vector<std::string>{ "./main", "19" });
		REQUIRE(arguments.foo == 19);
		REQUIRE(!arguments.bar.has_value());
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArgument>(
			std::vector<std::string>{ "./main", "91", "--bar", "true" });
		REQUIRE(arguments.foo == 91);
		REQUIRE(arguments.bar == true);
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArgument>(
			std::vector<std::string>{ "./main", "153", "-b", "true" });
		REQUIRE(arguments.foo == 153);
		REQUIRE(arguments.bar == true);
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArgument>(
			std::vector<std::string>{ "./main", "--bar", "true", "91" });
		REQUIRE(arguments.foo == 91);
		REQUIRE(arguments.bar == true);
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArgument>(
			std::vector<std::string>{ "./main", "-b", "true", "153" });
		REQUIRE(arguments.foo == 153);
		REQUIRE(arguments.bar == true);
	}
}

struct PositionalAndOptionalArrayArgument {
	std::array<int, 2> foo = {};
	std::optional<std::array<float, 3>> bar;
};

// NOLINTNEXTLINE
STRUCTOPT(PositionalAndOptionalArrayArgument, foo, bar);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional and optional array arguments" *
		  test_suite("single_positional_and_optional")) {
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArrayArgument>(
			std::vector<std::string>{ "./main", "1", "2" });
		REQUIRE(arguments.foo == std::array<int, 2>{ 1, 2 });
		REQUIRE(!arguments.bar.has_value());
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArrayArgument>(
			std::vector<std::string>{ "./main", "1", "5", "--bar", "1.1", "2.2", "3.3" });
		REQUIRE(arguments.foo == std::array<int, 2>{ 1, 5 });
		// NOLINTNEXTLINE
		REQUIRE(arguments.bar.value() == std::array<float, 3>{ 1.1F, 2.2F, 3.3F });
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArrayArgument>(
			std::vector<std::string>{ "./main", "1", "5", "-b", "1.1", "2.2", "3.3" });
		REQUIRE(arguments.foo == std::array<int, 2>{ 1, 5 });
		// NOLINTNEXTLINE
		REQUIRE(arguments.bar.value() == std::array<float, 3>{ 1.1F, 2.2F, 3.3F });
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArrayArgument>(
			std::vector<std::string>{ "./main", "--bar", "1.1", "2.2", "3.3", "1", "5" });
		REQUIRE(arguments.foo == std::array<int, 2>{ 1, 5 });
		// NOLINTNEXTLINE
		REQUIRE(arguments.bar.value() == std::array<float, 3>{ 1.1F, 2.2F, 3.3F });
	}
	{
		auto arguments = structopt::app("test").parse<PositionalAndOptionalArrayArgument>(
			std::vector<std::string>{ "./main", "-b", "1.1", "2.2", "3.3", "1", "5" });
		REQUIRE(arguments.foo == std::array<int, 2>{ 1, 5 });
		// NOLINTNEXTLINE
		REQUIRE(arguments.bar.value() == std::array<float, 3>{ 1.1F, 2.2F, 3.3F });
	}
}

struct ArrayVectorArgument {
	std::optional<std::array<float, 3>> point;
	std::vector<int> value;
};

// NOLINTNEXTLINE
STRUCTOPT(ArrayVectorArgument, point, value);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse single positional and optional arguments" *
		  test_suite("vector_positional")) {
	{
		auto arguments = structopt::app("test").parse<ArrayVectorArgument>(
			std::vector<std::string>{ "./main", "1", "2", "3" });
		REQUIRE(arguments.point.has_value() == false);
		REQUIRE(arguments.value == std::vector<int>{ 1, 2, 3 });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayVectorArgument>(
			std::vector<std::string>{ "./main", "--point", "1.1", "2.2", "3.3", "1", "2", "3" });
		REQUIRE(arguments.point == std::array<float, 3>{ 1.1F, 2.2F, 3.3F });
		REQUIRE(arguments.value == std::vector<int>{ 1, 2, 3 });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayVectorArgument>(
			std::vector<std::string>{ "./main", "-p", "1.1", "2.2", "3.3", "1", "2", "3" });
		REQUIRE(arguments.point == std::array<float, 3>{ 1.1F, 2.2F, 3.3F });
		REQUIRE(arguments.value == std::vector<int>{ 1, 2, 3 });
	}
}
