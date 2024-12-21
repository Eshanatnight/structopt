// MIT License
//
// Copyright (c) 2024 Eshan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "third_party/magic_enum.hpp"
#include "third_party/visit_struct.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <deque>
#include <exception>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <ostream>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define STRUCTOPT VISITABLE_STRUCT

namespace structopt {
	template<typename>
	struct array_size;
	template<typename T, size_t N>
	struct array_size<std::array<T, N>> {
		static size_t const size = N;
	};
}

// default specialization
namespace structopt {

	template<typename Test, template<typename...> class Ref>
	struct is_specialization : std::false_type {};

	template<template<typename...> class Ref, typename... Args>
	struct is_specialization<Ref<Args...>, Ref> : std::true_type {};
}

// specialize a type for all of the STL containers.
namespace structopt {
	namespace is_stl_container_impl {
		template<typename T>
		struct is_stl_container : std::false_type {};
		template<typename T, std::size_t N>
		struct is_stl_container<std::array<T, N>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::vector<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::deque<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::list<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::forward_list<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::set<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::multiset<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::map<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::multimap<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::unordered_set<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::unordered_multiset<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::unordered_map<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::unordered_multimap<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::stack<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::queue<Args...>> : std::true_type {};
		template<typename... Args>
		struct is_stl_container<std::priority_queue<Args...>> : std::true_type {};
	} // namespace is_stl_container_impl

	template<class T>
	struct is_array : std::is_array<T> {};
	template<class T, std::size_t N>
	struct is_array<std::array<T, N>> : std::true_type {};
	// optional:
	template<class T>
	struct is_array<T const> : is_array<T> {};
	template<class T>
	struct is_array<T volatile> : is_array<T> {};
	template<class T>
	struct is_array<T volatile const> : is_array<T> {};

	// type trait to utilize the implementation type traits as well as decay the type
	template<typename T>
	struct is_stl_container {
		static constexpr bool const value =
			is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
	};

}

namespace structopt::details {
	/// String functions
	static inline auto string_replace(
		std::string& str, const std::string& from, const std::string& whereTo) -> bool {
		const size_t start_pos = str.find(from);
		if(start_pos == std::string::npos) return false;
		str.replace(start_pos, from.length(), whereTo);
		return true;
	}

	static inline auto string_to_kebab(std::string_view str) -> std::string {
		std::string out(str);
		// Generate kebab case and present as option
		details::string_replace(out, "_", "-");
		return out;
	}
}

namespace structopt::details {
	/// number validation functions
	static inline auto is_binary_notation(std::string_view input) -> bool {
		return input.compare(0, 2, "0b") == 0 && input.size() > 2 &&
			   input.find_first_not_of("01", 2) == std::string_view::npos;
	}

	static inline auto is_hex_notation(std::string_view input) -> bool {
		return input.compare(0, 2, "0x") == 0 && input.size() > 2 &&
			   input.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string_view::npos;
	}

	static inline auto is_octal_notation(std::string_view input) -> bool {
		return input.compare(0, 1, "0") == 0 && input.size() > 1 &&
			   input.find_first_not_of("01234567", 1) == std::string_view::npos;
	}

	// NOLINTNEXTLINE
	static inline auto is_valid_number(std::string_view input) -> bool {
		if(is_binary_notation(input) || is_hex_notation(input) || is_octal_notation(input)) {
			return true;
		}

		if(input.empty()) {
			return false;
		}

		// NOLINTNEXTLINE
		std::size_t i = 0;
		// NOLINTNEXTLINE
		std::size_t j = input.length() - 1;

		// Handling whitespaces
		while(i < input.length() && input[i] == ' ') i++;
		while(input[j] == ' ') j--;

		if(i > j) return false;

		// if string is of length 1 and the only character is not a digit
		if(i == j && (input[i] < '0' || input[i] > '9')) return false;

		// If the 1st char is not '+', '-', '.' or digit
		if(input[i] != '.' && input[i] != '+' && input[i] != '-' &&
			(input[i] < '0' || input[i] > '9'))
			return false;

		// To check if a '.' or 'e' is found in given string. We use this flag to make sure that
		// either of them appear only once.
		bool dot_or_exp = false;

		for(; i <= j; i++) {
			// If any of the char does not belong to
			// {digit, +, -, ., e}
			if(input[i] != 'e' && input[i] != '.' && input[i] != '+' && input[i] != '-' &&
				(input[i] < '0' || input[i] > '9'))
				return false;

			if(input[i] == '.') {
				// checks if the char 'e' has already ccurred before '.' If yes, return false;.
				if(dot_or_exp) return false;

				// If '.' is the last character.
				if(i + 1 > input.length()) return false;

				// if '.' is not followed by a digit.
				if(input[i + 1] < '0' || input[i + 1] > '9') return false;
			}

			else if(input[i] == 'e') {
				// set dot_or_exp = 1 when e is encountered.
				dot_or_exp = true;

				// if there is no digit before 'e'.
				if(input[i - 1] < '0' || input[i - 1] > '9') return false;

				// If 'e' is the last Character
				if(i + 1 > input.length()) return false;

				// if e is not followed either by
				// '+', '-' or a digit
				if(input[i + 1] != '+' && input[i + 1] != '-' &&
					(input[i + 1] >= '0' && input[i] <= '9'))
					return false;
			}
		}

		return true;
	}
}

namespace structopt {
	class app;

	namespace details {
		struct visitor {
			std::string name;
			std::string version;
			std::optional<std::string> help;
			std::vector<std::string_view> field_names;
			std::deque<std::string_view> positional_field_names; // mutated by parser
			std::deque<std::string_view> positional_field_names_for_help;
			std::deque<std::string_view> vector_like_positional_field_names;
			std::deque<std::string_view> flag_field_names;
			std::deque<std::string_view> optional_field_names;
			std::deque<std::string_view> nested_struct_field_names;

			visitor() = default;

			explicit visitor(std::string name, std::string version) :
				name(std::move(name)), version(std::move(version)) {}

			explicit visitor(std::string name, std::string version, std::string help) :
				name(std::move(name)), version(std::move(version)), help(std::move(help)) {}

			// Visitor function for std::optional - could be an option or a flag
			template<typename T>
			auto operator()(std::string_view name, T&)
				-> std::enable_if_t<structopt::is_specialization<T, std::optional>::value, void> {
				field_names.emplace_back(name);
				if constexpr(std::is_same_v<typename T::value_type, bool>) {
					flag_field_names.emplace_back(name);
				} else {
					optional_field_names.emplace_back(name);
				}
			}

			// Visitor function for any positional field (not std::optional)
			template<typename T>
			auto operator()(std::string_view name, T&)
				-> std::enable_if_t<!structopt::is_specialization<T, std::optional>::value &&
										!visit_struct::traits::is_visitable<T>::value,
					void> {
				// BUG: not sanitizing the name may cause an issue down the line?
				field_names.emplace_back(name);
				positional_field_names.emplace_back(name);
				positional_field_names_for_help.emplace_back(name);
				if constexpr(structopt::is_specialization<T, std::deque>::value ||
							 structopt::is_specialization<T, std::list>::value ||
							 structopt::is_specialization<T, std::vector>::value ||
							 structopt::is_specialization<T, std::set>::value ||
							 structopt::is_specialization<T, std::multiset>::value ||
							 structopt::is_specialization<T, std::unordered_set>::value ||
							 structopt::is_specialization<T, std::unordered_multiset>::value ||
							 structopt::is_specialization<T, std::queue>::value ||
							 structopt::is_specialization<T, std::stack>::value ||
							 structopt::is_specialization<T, std::priority_queue>::value) {
					// keep track of vector-like fields as these (even though
					// positional) can be happy without any arguments
					vector_like_positional_field_names.emplace_back(name);
				}
			}

			// Visitor function for nested structs
			template<typename T>
			auto operator()(std::string_view name, T&)
				-> std::enable_if_t<visit_struct::traits::is_visitable<T>::value, void> {
				// name need to be sanitized to handle keywords(`new`, `delete`, `class`, etc.)
				// how would we identify a keyword? use some sort of convestion
				// say the var name needs to end with `_` char
				const auto _name = [&name]() -> std::string_view {
					if(name.back() == '_') {
						return name.substr(0, name.length() - 1);
					}

					return name;
				}();

				field_names.push_back(_name);
				nested_struct_field_names.push_back(_name);
			}

			auto is_field_name(std::string_view field_name) -> bool {
				return std::find(field_names.begin(), field_names.end(), field_name) !=
					   field_names.end();
			}

			// NOLINTNEXTLINE
			auto print_help(std::ostream& out_stream) const -> void {
				if(help.has_value() && !help.value().empty()) {
					out_stream << help.value();
				} else {
					out_stream << "\nUSAGE: " << name << " ";

					if(!flag_field_names.empty()) {
						out_stream << "[FLAGS] ";
					}

					if(!optional_field_names.empty()) {
						out_stream << "[OPTIONS] ";
					}

					if(!nested_struct_field_names.empty()) {
						out_stream << "[SUBCOMMANDS] ";
					}

					for(const auto& field: positional_field_names_for_help) {
						out_stream << field << " ";
					}

					bool has_h = false;
					bool has_v = false;
					if(!flag_field_names.empty()) {
						out_stream << "\n\nFLAGS:\n";
						for(const auto& flag: flag_field_names) {
							out_stream << "    -" << flag[0] << ", --" << flag << "\n";

							// NOLINTNEXTLINE
							switch(flag[0]) {
							case 'h' :
								has_h = true;
								break;
							case 'v' :
								has_v = true;
								break;
							}
						}
					} else {
						out_stream << "\n";
					}

					if(!optional_field_names.empty()) {
						out_stream << "\nOPTIONS:\n";
						for(const auto& option: optional_field_names) {

							// Generate kebab case and present as option
							auto kebab_case				= details::string_to_kebab(option);
							const std::string long_form = [&kebab_case, &option]() -> std::string {
								std::string str;
								if(kebab_case != option) {
									str = kebab_case;
								} else {
									str = option;
								}
								return str;
							}();

							if((has_v && option == "version") || (has_h && option == "help")) {
								out_stream << "    --" << long_form << " <" << option << ">\n";
							} else {
								out_stream << "    -" << option[0] << ", --" << long_form << " <"
										   << option << ">"
										   << "\n";
							}

							// NOLINTNEXTLINE
							switch(option[0]) {
							case 'h' :
								has_h = true;
								break;
							case 'v' :
								has_v = true;
								break;
							}
						}
					}

					if(!nested_struct_field_names.empty()) {
						out_stream << "\nSUBCOMMANDS:\n";
						for(const auto& str: nested_struct_field_names) {
							out_stream << "    " << str << "\n";
						}
					}

					if(!positional_field_names_for_help.empty()) {
						out_stream << "\nARGS:\n";
						for(const auto& arg: positional_field_names_for_help) {
							out_stream << "    " << arg << "\n";
						}
					}
				}
			}
		};
	}

}

namespace structopt {
	class exception : public std::exception {
		std::string what_;
		std::string help_;
		details::visitor visitor_;

	public:

		exception(std::string what, details::visitor visitor) :
			what_(std::move(what)), visitor_(std::move(visitor)) {
			std::stringstream out_sstream;
			visitor_.print_help(out_sstream);
			help_ = out_sstream.str();
		}

		[[nodiscard]] auto what() const noexcept -> const char* override {
			return what_.c_str();
		}

		[[nodiscard]] auto help() const noexcept -> const char* {
			return help_.c_str();
		}
	};
}

namespace structopt {
	namespace details {
		struct parser;
	}

	class sub_command {
		std::optional<bool> invoked_;
		details::visitor visitor_;

		friend struct structopt::details::parser;

	public:

		[[nodiscard]] constexpr auto has_value() const noexcept -> bool {
			return invoked_.has_value();
		}

		[[nodiscard]] explicit constexpr operator bool() const noexcept {
			return has_value();
		}
	};
}

namespace structopt::details {
	// Effectively s1.replace('-', '_') == s2
	inline auto equal_strings_replace_hyphens(std::string_view str1, std::string_view str2)
		-> bool {
		return std::equal(str1.begin(), str1.end(), str2.begin(), [](char char1, char char2) {
			return char1 == char2 || (char1 == '-' && char2 == '_');
		});
	}
}

namespace structopt::details {
	struct parser {
		using DetailsVisitor = structopt::details::visitor;
		DetailsVisitor visitor;
		std::vector<std::string> arguments;
		std::size_t current_index{ 1 };
		std::size_t next_index{ 1 };
		bool double_dash_encountered{ false }; // "--" option-argument delimiter
		bool sub_command_invoked{ false };
		std::string already_invoked_subcommand_name;

		explicit parser() = default;

		explicit parser(DetailsVisitor visitor_, std::vector<std::string> args) :
			visitor(std::move(visitor_)), arguments(std::move(args)) {}

		// NOLINTNEXTLINE easily swappable
		explicit parser(std::size_t next_idx,
			std::size_t curr_idx,
			bool double_dash_encountered,
			DetailsVisitor visitor_) :
			visitor(std::move(visitor_)),
			next_index(next_idx),
			current_index(curr_idx),
			double_dash_encountered(double_dash_encountered) {}

		auto is_optional(std::string_view name) -> bool {
			if(double_dash_encountered) {
				return false;
			}

			if(name == "--") {
				double_dash_encountered = true;
				return false;
			}

			if(is_valid_number(name)) {
				return false;
			}

			// return (name.size() >= 2 && name[0] == '-');
			bool result = false;
			if(name.size() >= 2) {
				// e.g., -b, -v
				if(name[0] == '-') {
					result = true;
					// e.g., --verbose
					if(name[1] == '-') {
						result = true;
					}
				}
			}
			return result;
		}

		// NOLINTNEXTLINE
		static auto is_kebab_case(std::string_view next, std::string_view field_name) -> bool {
			auto maybe_kebab_case = next;
			if(maybe_kebab_case.size() > 1 && maybe_kebab_case[0] == '-') {
				// remove first dash
				maybe_kebab_case.remove_prefix(1);
				if(maybe_kebab_case[0] == '-') {
					// there is a second leading dash remove it
					maybe_kebab_case.remove_prefix(1);
				}
				if(equal_strings_replace_hyphens(maybe_kebab_case, field_name)) {
					return true;
				}
			}
			return false;
		}

		static auto is_optional_field(std::string_view next, const std::string_view& field_name)
			-> bool {
			if((next.rfind('-', 0) == 0 && next.substr(1) == field_name) ||
				(next.rfind("--", 0) == 0 && next.substr(2) == field_name) ||
				(next == std::data({ '-', field_name[0], '\0' })))

				return true;

			return is_kebab_case(next, field_name);
		}

		auto is_optional_field(std::string_view next) -> bool {
			if(!is_optional(next)) {
				return false;
			}

			bool result = false;
			for(auto& field_name: visitor.field_names) {
				result = is_optional_field(next, field_name);
				if(result) {
					break;
				}
			}
			return result;
		}

		// checks if the next argument is a delimited optional field e.g., -std=c++17, where std
		// matches a field name and it is delimited by one of the two allowed delimiters: `=`
		// and `:` if true, the return value includes the delimiter that was used
		auto is_delimited_optional_argument(std::string_view next) -> std::pair<bool, char> {
			bool success   = false;
			char delimiter = '\0';

			auto equal_pos = next.find('=');
			auto colon_pos = next.find(':');

			// not delimited
			if(equal_pos == std::string_view::npos && colon_pos == std::string_view::npos) {
				return { success, delimiter };
			}

			const char chr = colon_pos < equal_pos ? ':' : '=';

			// split `next` into key and value check if key is an optional field
			// TODO: use substr??	// trim the -- or - from the left
			std::string key;
			bool delimiter_found = false;
			for(const char idx: next) {
				if(idx == chr && !delimiter_found) {
					delimiter		= chr;
					delimiter_found = true;
				} /* else if(idx == '-' && !delimiter_found) {
					continue;
				} */
				else if(!delimiter_found) {
					key += idx;
				}
			}

			// check if `key` is a valid optional field
			if(delimiter_found && is_optional_field(key)) {
				success = true;
			}

			return { success, delimiter };
		}

		static auto split_delimited_argument(char delimiter, std::string_view next)
			-> std::pair<std::string, std::string> {
			std::string key;
			std::string value;
			bool delimiter_found = false;

			// TODO: use substr??
			// Bug: wont you need to sanitize the - or -- from the left?
			for(const char idx: next) {
				if(idx == delimiter && !delimiter_found) {
					delimiter_found = true;
				} else {
					if(!delimiter_found) {
						key += idx;
					} else {
						value += idx;
					}
				}
			}
			return { key, value };
		}

		// Get the optional field name if any from
		// e.g., `-v` => `verbose`
		// e.g., `-log-level` => `log_level`
		auto get_full_optional_field_name(const std::string_view& next)
			-> std::optional<std::string_view> {
			std::optional<std::string_view> result;

			if(next.size() == 2 && next[0] == '-') {
				// short form of optional argument
				for(const auto& oarg: visitor.optional_field_names) {
					if(oarg[0] == next[1]) {
						// second character of next matches first character
						// of some optional field_name
						result = oarg;
						break;
					}
				}
			} else {
				// long form of optional argument strip dashes on the left
				const auto potential_field_name = next.substr(next.find_first_not_of('-'));

				// check if `potential_field_name` is in the optional field names list
				for(auto& oarg: visitor.optional_field_names) {
					if(equal_strings_replace_hyphens(potential_field_name, oarg)) {
						result = oarg;
						break;
					}
				}
			}

			return result;
		}

		template<typename T>
		auto parse_argument(std::string_view name) -> std::pair<T, bool> {
			if(next_index >= arguments.size()) {
				return { T(), false };
			}

			T result;
			bool success = true;

			if constexpr(visit_struct::traits::is_visitable<T>::value) {
				result = parse_nested_struct<T>(name);
			} else if constexpr(std::is_enum_v<T>) {
				result = parse_enum_argument<T>(name);
				next_index += 1;
			} else if constexpr(structopt::is_specialization<T, std::pair>::value) {
				result = parse_pair_argument<typename T::first_type, typename T::second_type>(name);
			} else if constexpr(structopt::is_specialization<T, std::tuple>::value) {
				result = parse_tuple_argument<T>(name);
			} else if constexpr(!is_stl_container<T>::value) {
				result = parse_single_argument<T>(name);
				next_index += 1;
			} else if constexpr(structopt::is_array<T>::value) {
				constexpr std::size_t SIZE = structopt::array_size<T>::size;
				result = parse_array_argument<typename T::value_type, SIZE>(name);
			} else if constexpr(structopt::is_specialization<T, std::deque>::value ||
								structopt::is_specialization<T, std::list>::value ||
								structopt::is_specialization<T, std::vector>::value) {
				result = parse_vector_like_argument<T>(name);
			} else if constexpr(structopt::is_specialization<T, std::set>::value ||
								structopt::is_specialization<T, std::multiset>::value ||
								structopt::is_specialization<T, std::unordered_set>::value ||
								structopt::is_specialization<T, std::unordered_multiset>::value) {
				result = parse_set_argument<T>(name);
			} else if constexpr(structopt::is_specialization<T, std::queue>::value ||
								structopt::is_specialization<T, std::stack>::value ||
								structopt::is_specialization<T, std::priority_queue>::value) {
				result = parse_container_adapter_argument<T>(name);
			} else {
				success = false;
			}
			return { result, success };
		}

		template<typename T>
		auto parse_optional_argument(std::string_view name) -> std::optional<T> {
			next_index += 1;

			if(next_index >= arguments.size()) {
				throw structopt::exception(
					"Error: expected value for optional argument `" + std::string{ name } + "`.",
					visitor);
			}

			auto [value, success] = parse_argument<T>(name);
			if(!success) {
				throw structopt::exception("Error: failed to correctly "
										   "parse optional argument `" +
											   std::string{ name } + "`.",
					visitor);
			}

			return value;
		}

		// Any field that can be constructed using std::stringstream Not container type
		// Not a visitable type, i.e., a nested struct
		template<typename T>
		auto parse_single_argument(std::string_view)
			-> std::enable_if_t<!visit_struct::traits::is_visitable<T>::value, T> {
			std::string argument = arguments[next_index];
			std::istringstream is_stream(argument);
			T result;

			if constexpr(std::is_integral_v<T>) {
				if(is_hex_notation(argument)) {
					is_stream >> std::hex >> result;
				} else if(is_octal_notation(argument)) {
					is_stream >> std::oct >> result;
				} else if(is_binary_notation(argument)) {
					argument.erase(0, 2); // remove "0b"
					result = static_cast<T>(std::stoi(argument, nullptr, 2));
				} else {
					is_stream >> std::dec >> result;
				}
			} else {
				is_stream >> result;
			}

			return result;
		}

		// Nested visitable struct
		template<typename T>
		auto parse_nested_struct(std::string_view name)
			-> std::enable_if_t<visit_struct::traits::is_visitable<T>::value, T> {
			T argument_struct;

			if constexpr(std::is_base_of_v<structopt::sub_command, T>) {
				argument_struct.invoked_ = true;
			}

			const auto field_name = [name] {
				if(name.back() == '_') return name.substr(0, name.length() - 1);
				return name;
			}();

			// Save struct field names
			argument_struct.visitor_.name	 = name; // sub-command name; not the program
			argument_struct.visitor_.version = visitor.version;
			visit_struct::for_each(argument_struct, argument_struct.visitor_);

			// add `help` and `version` optional arguments
			argument_struct.visitor_.optional_field_names.push_back("help");
			argument_struct.visitor_.optional_field_names.push_back("version");

			if(sub_command_invoked) {
				// a sub-command has already been invoked
				throw structopt::exception(
					"Error: failed to invoke sub-command `" + std::string{ name } +
						"` because a different sub-command, `" + already_invoked_subcommand_name +
						"`, has already been invoked.",
					argument_struct.visitor_);
			}

			sub_command_invoked				= true;
			already_invoked_subcommand_name = name;

			structopt::details::parser parser;
			parser.next_index			   = 0;
			parser.current_index		   = 0;
			parser.double_dash_encountered = double_dash_encountered;
			// need copy
			parser.visitor = argument_struct.visitor_;

			std::copy(arguments.begin() + next_index, // NOLINT (narrowing conversion)
				arguments.end(),
				std::back_inserter(parser.arguments));

			for(std::size_t i = 0; i < parser.arguments.size(); i++) {
				parser.current_index = i;
				visit_struct::for_each(argument_struct, parser);
			}

			// directly call the parser to check for `help` and `version` flags
			std::optional<bool> help	= false;
			std::optional<bool> version = false;
			for(std::size_t i = 0; i < parser.arguments.size(); i++) {
				parser.operator()("help", help);
				parser.operator()("version", version);

				if(help == true) {
					// if help is requested, print help and exit
					argument_struct.visitor_.print_help(std::cout);
					exit(EXIT_SUCCESS);
				} else if(version == true) {
					// if version is requested, print version and exit
					std::cout << argument_struct.visitor_.version << "\n";
					exit(EXIT_SUCCESS);
				}
			}

			// if all positional arguments were provided this list would be empty
			if(!parser.visitor.positional_field_names.empty()) {
				for(auto& field_name: parser.visitor.positional_field_names) {
					if(std::find(parser.visitor.vector_like_positional_field_names.begin(),
						   parser.visitor.vector_like_positional_field_names.end(),
						   field_name) == parser.visitor.vector_like_positional_field_names.end()) {
						// this positional argument is not a vector-like argument it expects
						// value(s)
						throw structopt::exception(
							"Error: expected value for positional argument `" +
								std::string(field_name) + "`.",
							argument_struct.visitor_);
					}
				}
			}

			// update current and next
			current_index += parser.next_index;
			next_index += parser.next_index;

			return argument_struct;
		}

		// Pair argument
		template<typename T1, typename T2>
		auto parse_pair_argument(std::string_view name) -> std::pair<T1, T2> {
			std::pair<T1, T2> result;
			{
				// Pair first
				auto [value, success] = parse_argument<T1>(name);
				if(success) {
					result.first = value;
				} else {
					if(next_index == arguments.size()) {

						// end of arguments list first argument not provided
						throw structopt::exception("Error: failed to correctly parse the pair `" +
													   std::string{ name } +
													   "`. Expected 2 arguments, 0 provided.",
							visitor);
					}

					throw structopt::exception("Error: failed to correctly parse first "
											   "element of pair `" +
												   std::string{ name } + "`",
						visitor);
				}
			}
			{
				// Pair second
				auto [value, success] = parse_argument<T2>(name);
				if(success) {
					result.second = value;
				} else {
					if(next_index == arguments.size()) {

						// end of arguments list second argument not provided
						throw structopt::exception("Error: failed to correctly parse the pair `" +
													   std::string{ name } +
													   "`. Expected 2 arguments, only 1 provided.",
							visitor);
					}

					throw structopt::exception("Error: failed to correctly parse second "
											   "element of pair `" +
												   std::string{ name } + "`",
						visitor);
				}
			}
			return result;
		}

		// Array argument
		template<typename T, std::size_t N>
		auto parse_array_argument(std::string_view name) -> std::array<T, N> {
			std::array<T, N> result{};

			const auto arguments_left = arguments.size() - next_index;
			if(arguments_left == 0 || arguments_left < N) {
				throw structopt::exception("Error: expected " + std::to_string(N) +
											   " values for std::array argument `" +
											   std::string(name) + "` - instead got only " +
											   std::to_string(arguments_left) + " arguments.",
					visitor);
			}

			for(std::size_t i = 0; i < N; i++) {
				auto [value, success] = parse_argument<T>(name);
				if(success) {
					result[i] = value;
				}
			}
			return result;
		}

		template<class Tuple, class F, std::size_t... I>
		constexpr auto for_each_impl(Tuple&& tuple, F&& func, std::index_sequence<I...>) -> F {
			return (void)std::initializer_list<int>{ (
					   std::forward<F>(func)(std::get<I>(std::forward<Tuple>(tuple))), 0)... },
				   func;
		}

		template<class Tuple, class F>
		constexpr auto for_each(Tuple&& tuple, F&& func) -> F {
			return for_each_impl(std::forward<Tuple>(tuple),
				std::forward<F>(func),
				std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
		}

		// Parse single tuple element
		template<typename T>
		void parse_tuple_element(
			std::string_view name, std::size_t index, std::size_t size, T&& result) {
			auto [value, success] = parse_argument<std::remove_reference_t<T>>(name);
			if(!success) {
				if(next_index == arguments.size()) {
					// end of arguments list failed to parse tuple <>. expected `size`
					// arguments, `index` provided
					throw structopt::exception("Error: failed to correctly parse tuple `" +
												   std::string{ name } + "`. Expected " +
												   std::to_string(size) + " arguments, " +
												   std::to_string(index) + " provided.",
						visitor);
				}

				throw structopt::exception("Error: failed to correctly parse tuple `" +
											   std::string{ name } +
											   "` {size = " + std::to_string(size) + "} at index " +
											   std::to_string(index) + ".",
					visitor);
			}

			result = value;
		}

		// Tuple argument
		template<typename Tuple>
		auto parse_tuple_argument(std::string_view name) -> Tuple {
			Tuple result;
			std::size_t idx			  = 0;
			constexpr auto tuple_size = std::tuple_size<Tuple>::value;
			for_each(result, [&](auto&& arg) {
				parse_tuple_element(name, idx, tuple_size, arg);
				idx += 1;
			});
			return result;
		}

		// Vector, deque, list
		template<typename T>
		auto parse_vector_like_argument(std::string_view name) -> T {
			T result;

			// Parse from current till end
			while(next_index < arguments.size()) {
				const std::string_view next = arguments[next_index];
				if(is_optional_field(next) || next == "--" ||
					is_delimited_optional_argument(next).first) {
					if(next == "--") {
						double_dash_encountered = true;
						next_index += 1;
					}
					// this marks the end of the container (break here)
					break;
				}

				auto [value, success] = parse_argument<typename T::value_type>(name);
				if(success) {
					result.push_back(value);
				}
			}
			return result;
		}

		// stack, queue, priority_queue
		template<typename T>
		auto parse_container_adapter_argument(std::string_view name) -> T {
			T result;
			// Parse from current till end
			while(next_index < arguments.size()) {
				const std::string_view next = arguments[next_index];
				if(is_optional_field(next) || next == "--" ||
					is_delimited_optional_argument(next).first) {
					if(next == "--") {
						double_dash_encountered = true;
						next_index += 1;
					}
					// this marks the end of the container (break here)
					break;
				}
				auto [value, success] = parse_argument<typename T::value_type>(name);
				if(success) {
					result.push(value);
				}
			}
			return result;
		}

		// Set, multiset, unordered_set, unordered_multiset
		template<typename T>
		auto parse_set_argument(std::string_view name) -> T {
			T result;
			// Parse from current till end
			while(next_index < arguments.size()) {
				const std::string_view next = arguments[next_index];
				if(is_optional_field(next) || next == "--" ||
					is_delimited_optional_argument(next).first) {
					if(next == "--") {
						double_dash_encountered = true;
						next_index += 1;
					}
					// this marks the end of the container (break here)
					break;
				}
				auto [value, success] = parse_argument<typename T::value_type>(name);
				if(success) {
					result.insert(value);
				}
			}
			return result;
		}

		// Enum class
		template<typename T>
		auto parse_enum_argument(std::string_view name) -> T {
			const auto maybe_enum_value = magic_enum::enum_cast<T>(arguments[next_index]);
			if(maybe_enum_value.has_value()) {
				return maybe_enum_value.value();
			}
			constexpr auto allowed_names = magic_enum::enum_names<T>();

			std::string allowed_names_string;
			if(allowed_names.size()) {
				for(size_t i = 0; i < allowed_names.size() - 1; i++) {
					allowed_names_string += std::string{ allowed_names[i] } + ", ";
				}
				allowed_names_string += allowed_names[allowed_names.size() - 1];
			}

			throw structopt::exception("Error: unexpected input `" +
										   std::string{ arguments[next_index] } +
										   "` provided for enum argument `" + std::string{ name } +
										   "`. Allowed values are {" + allowed_names_string + "}",
				visitor);
			// TODO: Throw error invalid enum option
		}

		// Visitor function for nested struct
		template<typename T>
		auto operator()(std::string_view name, T& value)
			-> std::enable_if_t<visit_struct::traits::is_visitable<T>::value, void> {
			current_index = std::max(next_index, current_index);

			if(current_index >= arguments.size()) {
				return;
			}

			const std::string& next = arguments[current_index];
			// sanitize the field name
			const std::string_view field_name = [&name] {
				if(name.back() == '_') return name.substr(0, name.length() - 1);
				return name;
			}();

			// Check if `next` is the start of a subcommand
			if(visitor.is_field_name(next) && next == field_name) {
				next_index += 1;
				value = parse_nested_struct<T>(name);
			}
		}

		// Visitor function for any positional field (not std::optional)
		template<typename T>
		auto operator()(std::string_view name, T& result)
			-> std::enable_if_t<!structopt::is_specialization<T, std::optional>::value &&
									!visit_struct::traits::is_visitable<T>::value,
				void> {
			current_index = std::max(next_index, current_index);

			if(current_index >= arguments.size()) return;

			const std::string next = arguments[current_index];

			if(is_optional(next)) return;

			if(visitor.positional_field_names.empty()) {
				// We're not looking to save any more positional fields all of them already
				// have a value
				throw structopt::exception(
					"Error: unexpected argument '" + std::string(next) + "'", visitor);
			}

			const auto field_name = visitor.positional_field_names.front();

			// if(field_name != name) {
			if(field_name != std::string{ name }) { // TODO: sanity check remove later
				// current field is not the one we want to parse
				return;
			}

			// Remove from the positional field list as it is about to be parsed
			visitor.positional_field_names.pop_front();

			auto [value, success] = parse_argument<T>(field_name);
			if(success) {
				result = value;
			} else {
				// positional field does not yet have a value
				visitor.positional_field_names.push_front(field_name);
			}
		}

		// Visitor function for std::optional field
		template<typename T>
		// NOLINTNEXTLINE
		auto operator()(std::string_view name, T& value)
			-> std::enable_if_t<structopt::is_specialization<T, std::optional>::value, void> {
			current_index = std::max(next_index, current_index);

			if(current_index < arguments.size()) {
				const auto next		  = arguments[current_index];
				const auto field_name = std::string{ name };

				if(next == "--" && !double_dash_encountered) {
					double_dash_encountered = true;
					next_index += 1;
					return;
				}

				// if `next` looks like an optional argument
				// i.e., starts with `-` or `--`
				// see if you can find an optional field in the struct with a matching name

				// check if the current argument looks like it could be this optional field
				if(!double_dash_encountered && is_optional_field(next, field_name)) {

					// this is an optional argument matching the current struct field
					if constexpr(std::is_same_v<typename T::value_type, bool>) {
						// It is a boolean optional argument
						// Does it have a default value?
						// If yes, this is a FLAG argument, e.g,, "--verbose" will set it to
						// true if the default value is false No need to write "--verbose true"
						if(value.has_value()) {
							// The field already has a default value!
							value = !value.value(); // simply toggle it
							next_index += 1;
						} else {
							// boolean optional argument doesn't have a default value
							// expect one
							value = parse_optional_argument<typename T::value_type>(name);
						}
					} else {
						// Not std::optional<bool>
						// Parse the argument type <T>
						value = parse_optional_argument<typename T::value_type>(name);
					}
				} else {
					if(!double_dash_encountered) {

						// maybe this is an optional argument that is delimited with '=' or ':'
						// e.g., --foo=bar or --foo:BAR
						if(next.size() > 1 && next[0] == '-') {
							const auto [success, delimiter] = is_delimited_optional_argument(next);
							if(success) {
								const auto [lhs, rhs] = split_delimited_argument(delimiter, next);
								// update next_index and return
								// the parser will take care of the rest

								// if `lhs` is an optional argument (i.e., maps to an optional
								// field in the original struct), then insert into arguments
								// list
								auto potential_field_name = get_full_optional_field_name(lhs);
								if(potential_field_name.has_value()) {
									for(const auto& arg: { rhs, lhs }) {
										const auto begin = arguments.begin();
										// NOLINTNEXTLINE
										arguments.insert(begin + next_index + 1, arg);
									}
								}
								// get past the current argument, e.g., `--foo=bar`
								next_index += 1;
								return;
							}
						}

						// A direct match of optional argument with field_name has not happened
						// This _could_ be a combined argument
						// e.g., -abc => -a, -b, and -c where each of these is a flag argument

						std::vector<std::string> potential_combined_argument;

						if(!is_optional_field(next) && next[0] == '-' &&
							(next.size() > 1 && next[1] != '-')) {
							for(std::size_t i = 1; i < next.size(); i++) {
								potential_combined_argument.push_back(
									"-" + std::string(1, next[i]));
							}
						}

						if(!potential_combined_argument.empty()) {
							bool is_combined_argument = true;
							for(auto& arg: potential_combined_argument) {
								if(!is_optional_field(arg)) {
									is_combined_argument = false;
									// TODO: report error unrecognized option in combined
									// argument
								}
							}

							if(is_combined_argument) {

								// check and make sure the current field_name is
								// in `potential_combined_argument`
								//
								// Let's say the argument `next` is `-abc`
								// the current field name is `b`
								// 1. Split `-abc` into `-a`, `-b`, and `-c`
								// 2. Check if `-b` is in the above list
								//    1. If yes, consider this as a combined argument
								//       push the list of arguments (split up) into `arguments`
								//    2. If no, nothing to do here
								bool field_name_matched = false;
								for(auto& arg: potential_combined_argument) {
									if(arg == "-" + std::string(1, field_name[0])) {
										field_name_matched = true;
									}
								}

								if(field_name_matched) {
									// confirmed: this is a combined argument

									// insert the individual options that make up the combined
									// argument right after the combined argument e.g., ""./main
									// -abc" becomes "./main -abc -a -b -c" Once this is done,
									// increment `next_index` so that the parser loop will
									// service
									// `-a`, `-b` and `-c` like any other optional arguments
									// (flags and otherwise)
									for(auto it = potential_combined_argument.rbegin();
										it != potential_combined_argument.rend();
										++it) {
										auto& arg = *it;
										if(next_index < arguments.size()) {
											auto begin = arguments.begin();
											// NOLINTNEXTLINE
											arguments.insert(begin + next_index + 1, arg);
										} else {
											arguments.push_back(arg);
										}
									}

									// get past the current combined argument
									next_index += 1;
								}
							}
						}
					}
				}
			}
		}
	};

	// Specialization for std::string
	template<>
	inline auto parser::parse_single_argument<std::string>(std::string_view) -> std::string {
		return arguments[next_index];
	}

	// Specialization for bool
	// yes, YES, on, 1, true, TRUE, etc. = true
	// no, NO, off, 0, false, FALSE, etc. = false
	// Converts argument to lower case before check
	template<>
	inline auto parser::parse_single_argument<bool>(std::string_view name) -> bool {
		current_index = std::max(next_index, current_index);

		if(current_index >= arguments.size()) {
			return false;
		}

		static constexpr std::array<std::string_view, 4> true_strings{ "on", "yes", "1", "true" };
		static constexpr std::array<std::string_view, 4> false_strings{ "off", "no", "0", "false" };

		std::string current_argument = std::string(arguments[current_index]);

		// Convert argument to lower case
		std::transform(current_argument.begin(),
			current_argument.end(),
			current_argument.begin(),
			[](unsigned char chr) { return static_cast<char>(std::tolower(chr)); });

		// Detect if argument is true or false
		if(std::find(true_strings.begin(), true_strings.end(), current_argument) !=
			true_strings.end()) {
			return true;
		}
		if(std::find(false_strings.begin(), false_strings.end(), current_argument) !=
			false_strings.end()) {
			return false;
		}
		throw structopt::exception("Error: failed to parse boolean argument `" +
									   std::string{ name } + "`." + " `" + current_argument + "`" +
									   " is invalid.",
			visitor);
	}
}

namespace structopt {
	class app {
		details::visitor visitor;

	public:

		explicit app(std::string name, std::string version = "", std::string help = "") :
			visitor(std::move(name), std::move(version), std::move(help)) {}

		template<typename T>
		// NOLINTNEXTLINE
		[[nodiscard]] auto parse(const std::vector<std::string>& arguments) -> T {
			T argument_struct = T();

			// Visit the struct and save flag, optional and positional field names
			visit_struct::for_each(argument_struct, visitor);

			// add `help` and `version` optional arguments
			visitor.optional_field_names.emplace_back("help");
			visitor.optional_field_names.emplace_back("version");

			// Construct the argument parser
			// structopt::details::parser parser(visitor, arguments);
			structopt::details::parser parser;
			parser.arguments = arguments;
			parser.visitor	 = visitor;

			for(std::size_t i = 1; i < parser.arguments.size(); i++) {
				parser.current_index = i;
				visit_struct::for_each(argument_struct, parser);
			}

			// directly call the parser to check for `help` and `version` flags
			std::optional<bool> help	= false;
			std::optional<bool> version = false;
			for(std::size_t i = 1; i < parser.arguments.size(); i++) {
				parser.operator()("help", help);
				parser.operator()("version", version);

				if(help == true) {
					// if help is requested, print help and exit
					visitor.print_help(std::cout);
					exit(EXIT_SUCCESS);
				} else if(version == true) {
					// if version is requested, print version and exit
					std::cout << visitor.version << "\n";
					exit(EXIT_SUCCESS);
				}
			}

			// if all positional arguments were provided this list would be empty
			if(!parser.visitor.positional_field_names.empty()) {
				for(const auto& field_name: parser.visitor.positional_field_names) {
					if(std::find(parser.visitor.vector_like_positional_field_names.begin(),
						   parser.visitor.vector_like_positional_field_names.end(),
						   field_name) == parser.visitor.vector_like_positional_field_names.end()) {
						// this positional argument is not a vector-like argument it expects
						// value(s)
						throw structopt::exception(
							"Error: expected value for positional argument `" +
								std::string(field_name) + "`.",
							parser.visitor);
					}
				}
			}

			if(parser.current_index < parser.arguments.size()) {
				throw structopt::exception("Error: unrecognized argument '" +
											   std::string(parser.arguments[parser.current_index]) +
											   "'",
					parser.visitor);
			}

			return argument_struct;
		}

		template<typename T>
		// NOLINTNEXTLINE intentional
		[[nodiscard]] auto parse(int argc, char* argv[]) -> T {
			std::vector<std::string> arguments{ argv, argv + argc };
			return parse<T>(arguments);
		}

		[[nodiscard]] auto help() const -> std::string {
			std::stringstream str_stream;
			visitor.print_help(str_stream);
			return str_stream.str();
		}
	};

} // namespace structopt
