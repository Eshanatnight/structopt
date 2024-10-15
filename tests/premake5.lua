--- @type utils
local utils = dofile("scripts/utils.lua")

--- wrapper over utils.makeWorkspace
--- @param name string
local function MakeWorkspace(name)
	local buildScriptLocation = "../build/" .. "%{wks.name}/"
	local configs = { "Debug" }
	local arch = { "x64", "arm64" }
	local preProcessorDefines = {"_CRT_SECURE_NO_WARNINGS" }
	utils.makeWorkspace(name, buildScriptLocation, configs, arch, preProcessorDefines)
end

local sources = {
	"main.cpp",
	"test_single_positional_argument.cpp",
	"test_array_positional_argument.cpp",
	"test_single_optional_argument.cpp",
	"test_array_optional_argument.cpp",
	"test_single_positional_and_optional.cpp",
	"test_nested_struct.cpp",
	"test_multiple_nested_structs_positional.cpp",
	"test_vector_positional_argument.cpp",
	"test_vector_optional_argument.cpp",
	"test_optional_flag_argument.cpp",
	"test_enum_positional_argument.cpp",
	"test_enum_optional_argument.cpp",
	"test_optional_argument_naming.cpp",
	"test_double_dash_option_delimiter.cpp",
	"test_ristricted_arguments.cpp",
	"test_trailing_positional_arguments.cpp",
	"test_tuple_positional_argument.cpp",
	"test_tuple_optional_argument.cpp",
	"test_deque_positional_argument.cpp",
	"test_deque_optional_argument.cpp",
	"test_set_positional_argument.cpp",
	"test_list_positional_argument.cpp",
	"test_container_adapter_positional_argument.cpp",
	"test_combined_flag_arguments.cpp",
	"test_equal_delimiter_optional_argument.cpp",
	"test_option_delimiter.cpp",
	"test_excess_positional_arguments.cpp",
	"test_unknown_optional_argument.cpp",
	-- "test_3d_array_positional_argument.cpp",
}

local OBJECTDIR = "../build/" .. "%{wks.name}/"  .. "bin-int/"  .. "/%{prj.name}"
local TARGETDIR = "../build/" .. "%{wks.name}/"  .. "bin/" .. "/%{prj.name}"

MakeWorkspace("structopt_tests")

project("structopt_tests")
kind(utils.BUILDKIND)
language(utils.LANGUAGE)
cppdialect(utils.STANDARD)
targetdir(TARGETDIR)
objdir(OBJECTDIR)
includedirs({ "../src", "./" })
files(sources)
filter("files:main.cpp")
defines({ "DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN" })