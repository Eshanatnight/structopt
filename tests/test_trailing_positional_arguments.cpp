#include <doctest.hpp>
#include <structopt.hpp>

using doctest::test_suite;

struct Options {
	// positional argument
	//   e.g., ./main <file>
	std::string config_file;

	// optional argument
	//   e.g., -b "192.168.5.3"
	//   e.g., --bind_address "192.168.5.3"
	// The long option can be passed in kebab case
	//   e.g., --bind-address "192.168.5.3"
	std::optional<std::string> bind_address;

	// Use `std::optional<bool>` and provide a default value.
	// Now you have a flag!
	//   e.g., -v
	//   e.g., --verbose
	// Passing this flag will set this
	// value to (!default_value), i.e., true
	std::optional<bool> verbose = false;

	// structopt also support defining enum classes
	// The argument (string) will be converted (if possible)
	// into the equivalent enum value
	//   e.g., --log-level debug
	//   e.g., -l error
	enum class LogLevel {
		debug,
		info,
		warn,
		error,
		critical
	};
	std::optional<LogLevel> log_level = LogLevel::info;

	// Here, structopt will check for `-u` or `--user`
	// and parse the next 2 arguments into an `std::pair`
	std::optional<std::pair<std::string, std::string>> user;

	// You can use containers like std::vector
	// to save variadic arguments of some type
	std::vector<std::string> files;
};

// NOLINTNEXTLINE
STRUCTOPT(Options, config_file, bind_address, verbose, log_level, user, files);

// NOLINTNEXTLINE
TEST_CASE("structopt can parse trailing positional arguments" * test_suite("test_trailing_args")) {
	{
		auto arguments = structopt::app("test").parse<Options>(std::vector<std::string>{ "./main",
			"--bind-address",
			"localhost:9000",
			"-v",
			"-log-level",
			"error",
			"--user",
			"Pranav Kumar",
			"pranav.kumar@foo.com",
			"config.csv" });
		REQUIRE(arguments.config_file == "config.csv");
		REQUIRE(arguments.bind_address == "localhost:9000");
		REQUIRE(arguments.verbose == true);
		REQUIRE(arguments.log_level == Options::LogLevel::error);
		REQUIRE(arguments.user ==
				std::pair<std::string, std::string>{ "Pranav Kumar", "pranav.kumar@foo.com" });
		REQUIRE(arguments.files == std::vector<std::string>{});
	}
	{
		auto arguments = structopt::app("test").parse<Options>(std::vector<std::string>{ "./main",
			"config_2.csv",
			"--bind-address",
			"192.168.7.3",
			"-log-level=debug",
			"file1.txt",
			"file2.txt",
			"file3.txt",
			"file4.txt",
			"--user",
			"John Doe",
			"john.doe@foo.com" });
		REQUIRE(arguments.config_file == "config_2.csv");
		REQUIRE(arguments.bind_address == "192.168.7.3");
		REQUIRE(arguments.verbose == false);
		REQUIRE(arguments.log_level == Options::LogLevel::debug);
		REQUIRE(arguments.user ==
				std::pair<std::string, std::string>{ "John Doe", "john.doe@foo.com" });
		REQUIRE(arguments.files ==
				std::vector<std::string>{ "file1.txt", "file2.txt", "file3.txt", "file4.txt" });
	}
	{
		auto arguments = structopt::app("test").parse<Options>(std::vector<std::string>{ "./main",
			"config_2.csv",
			"--bind-address",
			"192.168.7.3",
			"-log-level",
			"debug",
			"file1.txt",
			"file2.txt",
			"file3.txt",
			"file4.txt",
			"--user",
			"John Doe",
			"john.doe@foo.com" });
		REQUIRE(arguments.config_file == "config_2.csv");
		REQUIRE(arguments.bind_address == "192.168.7.3");
		REQUIRE(arguments.verbose == false);
		REQUIRE(arguments.log_level == Options::LogLevel::debug);
		REQUIRE(arguments.user ==
				std::pair<std::string, std::string>{ "John Doe", "john.doe@foo.com" });
		REQUIRE(arguments.files ==
				std::vector<std::string>{ "file1.txt", "file2.txt", "file3.txt", "file4.txt" });
	}
}
