#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct EnumOptionalArgument {
	enum class Color {
		red,
		blue,
		green
	};
	std::optional<Color> color;
};

// NOLINTNEXTLINE
STRUCTOPT(EnumOptionalArgument, color);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse enum class optional argument" * test_suite("enum_optional")) {
	{
		auto arguments = structopt::app("test").parse<EnumOptionalArgument>(
			std::vector<std::string>{ "./main", "--color", "red" });
		REQUIRE(arguments.color == EnumOptionalArgument::Color::red);
	}
	{
		auto arguments = structopt::app("test").parse<EnumOptionalArgument>(
			std::vector<std::string>{ "./main", "-c", "blue" });
		REQUIRE(arguments.color == EnumOptionalArgument::Color::blue);
	}
}

struct MultipleEnumOptionalArguments {
	enum class Color {
		red,
		blue,
		green
	};
	std::optional<Color> color;

	enum class TextAlign {
		left,
		middle,
		right
	};
	std::optional<TextAlign> text_align;
};

// NOLINTNEXTLINE
STRUCTOPT(MultipleEnumOptionalArguments, color, text_align);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse enum class optional argument" * test_suite("enum_optional")) {
	{
		auto arguments = structopt::app("test").parse<MultipleEnumOptionalArguments>(
			std::vector<std::string>{ "./main", "--color", "red", "--text_align", "left" });
		REQUIRE(arguments.color == MultipleEnumOptionalArguments::Color::red);
		REQUIRE(arguments.text_align == MultipleEnumOptionalArguments::TextAlign::left);
	}
	{
		auto arguments = structopt::app("test").parse<MultipleEnumOptionalArguments>(
			std::vector<std::string>{ "./main", "--text_align", "middle", "--color", "green" });
		REQUIRE(arguments.color == MultipleEnumOptionalArguments::Color::green);
		REQUIRE(arguments.text_align == MultipleEnumOptionalArguments::TextAlign::middle);
	}
}
