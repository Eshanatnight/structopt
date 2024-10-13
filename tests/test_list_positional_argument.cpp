#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct ListIntArgument {
	std::list<int> value;
};

// NOLINTNEXTLINE
STRUCTOPT(ListIntArgument, value);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse list positional argument" * test_suite("list_positional")) {
	// List of ints
	{
		auto arguments = structopt::app("test").parse<ListIntArgument>(
			std::vector<std::string>{ "./main", "1", "2", "3" });
		REQUIRE(arguments.value == std::list<int>{ 1, 2, 3 });
	}
}

struct ListIntArgumentWithOtherFlags {
	std::list<int> value;
	std::optional<bool> foo = false;
};

// NOLINTNEXTLINE
STRUCTOPT(ListIntArgumentWithOtherFlags, value, foo);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse list positional argument" * test_suite("list_positional")) {
	{
		auto arguments = structopt::app("test").parse<ListIntArgumentWithOtherFlags>(
			std::vector<std::string>{ "./main", "1", "2", "3", "--foo" });
		REQUIRE(arguments.value == std::list<int>{ 1, 2, 3 });
		REQUIRE(arguments.foo == true);
	}
	{
		auto arguments = structopt::app("test").parse<ListIntArgumentWithOtherFlags>(
			std::vector<std::string>{ "./main", "--foo", "1", "2", "3" });
		REQUIRE(arguments.value == std::list<int>{ 1, 2, 3 });
		REQUIRE(arguments.foo == true);
	}
	{
		bool exception_thrown{ false };
		try {
			auto arguments = structopt::app("test").parse<ListIntArgumentWithOtherFlags>(
				std::vector<std::string>{ "./main", "1", "2", "--foo", "3" });
			REQUIRE(arguments.value == std::list<int>{ 1, 2 });
			REQUIRE(arguments.foo == true);
		} catch(structopt::exception& e) {
			exception_thrown = true;
		}
		REQUIRE(exception_thrown == true);
	}
}
