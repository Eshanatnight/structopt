#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct ArrayIntArgument {
	std::array<int, 3> value = {};
};

struct ArrayFloatArgument {
	std::array<float, 5> value = {};
};

struct ArrayCharArgument {
	std::array<char, 2> value = {};
};

struct ArrayBoolArgument {
	std::array<bool, 8> value = {};
};

struct ArrayStringArgument {
	std::array<std::string, 4> value = {};
};

// NOLINTNEXTLINE
STRUCTOPT(ArrayIntArgument, value);
// NOLINTNEXTLINE
STRUCTOPT(ArrayFloatArgument, value);
// NOLINTNEXTLINE
STRUCTOPT(ArrayCharArgument, value);
// NOLINTNEXTLINE
STRUCTOPT(ArrayBoolArgument, value);
// NOLINTNEXTLINE
STRUCTOPT(ArrayStringArgument, value);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse fixed array positional argument" * test_suite("array_positional")) {
	// Array of ints
	{
		auto arguments = structopt::app("test").parse<ArrayIntArgument>(
			std::vector<std::string>{ "./main", "1", "2", "3" });
		REQUIRE(arguments.value == std::array<int, 3>{ 1, 2, 3 });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayIntArgument>(
			std::vector<std::string>{ "./main", "-1", "2", "-3" });
		REQUIRE(arguments.value == std::array<int, 3>{ -1, 2, -3 });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayIntArgument>(
			std::vector<std::string>{ "./main", "-1.1", "2.2", "-3.3" });
		REQUIRE(arguments.value == std::array<int, 3>{ -1, 2, -3 });
	}

	// Array of floats
	{
		auto arguments = structopt::app("test").parse<ArrayFloatArgument>(
			std::vector<std::string>{ "./main", "1.1", "2.2", "3.3", "4.4", "5.5" });
		REQUIRE(arguments.value == std::array<float, 5>{ 1.1F, 2.2F, 3.3F, 4.4F, 5.5F });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayFloatArgument>(
			std::vector<std::string>{ "./main", "1.1", "-2.2", "3.3", "-4.4", ".5" });
		REQUIRE(arguments.value == std::array<float, 5>{ 1.1F, -2.2F, 3.3F, -4.4F, 0.5F });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayFloatArgument>(
			std::vector<std::string>{ "./main", "0.1E1", "-0.2e2", "+0.3E3", "-0.4e4", "+5e5" });
		REQUIRE(
			arguments.value == std::array<float, 5>{ 0.1e1F, -0.2e2F, +0.3e3F, -0.4e4F, +5e5F });
	}
	{
		auto arguments = structopt::app("test").parse<ArrayFloatArgument>(
			std::vector<std::string>{ "./main", ".1", "-.2", "+.3", "-.4", "+.5" });
		REQUIRE(arguments.value == std::array<float, 5>{ 0.1F, -0.2F, 0.3F, -0.4F, 0.5F });
	}

	// Array of chars
	{
		auto arguments = structopt::app("test").parse<ArrayCharArgument>(
			std::vector<std::string>{ "./main", "a", "b" });
		REQUIRE(arguments.value == std::array<char, 2>{ 'a', 'b' });
	}

	// Array of bools
	{
		auto arguments = structopt::app("test").parse<ArrayBoolArgument>(std::vector<std::string>{
			"./main", "true", "false", "on", "off", "yes", "no", "1", "0" });
		REQUIRE(arguments.value ==
				std::array<bool, 8>{ true, false, true, false, true, false, true, false });
	}

	// Array of strings
	{
		auto arguments = structopt::app("test").parse<ArrayStringArgument>(
			std::vector<std::string>{ "./main", "abc", "def", "ghi", "jkl" });
		REQUIRE(arguments.value == std::array<std::string, 4>{ "abc", "def", "ghi", "jkl" });
	}

	// 0 arguments provided
	{
		bool exception_thrown = false;
		try {
			[[maybe_unused]] auto ignore = structopt::app("test").parse<ArrayIntArgument>(
				std::vector<std::string>{ "./main" });
		} catch(structopt::exception&) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown == true);
	}
}
