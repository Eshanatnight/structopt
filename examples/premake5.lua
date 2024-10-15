--- @type utils
local utils = include("scripts/utils.lua")

--- wrapper around utils.makeProject
---@param name string
---@param files string[]
local function MakeProject(name, files)
	local STRUCTOPT_SRC = { "../src" }
	utils.makeProject(name, files, STRUCTOPT_SRC)
end

--- wrapper over utils.makeWorkspace
--- @param name string
local function MakeWorkspace(name)
	local buildScriptLocation = "../build/" .. "%{wks.name}/"
	local configs = { "Debug" }
	local arch = { "x64" }
	utils.makeWorkspace(name, buildScriptLocation, configs, arch)
end

MakeWorkspace("structopt_examples")

MakeProject("demo", { "./demo.cpp" })
MakeProject("compound_arguments", { "./compound_arguments.cpp" })
MakeProject("double_dash_argument", { "./double_dash_argument.cpp" })
MakeProject("enum_class_argument", { "./enum_class_argument.cpp" })
MakeProject("flag_arguments", { "./flag_arguments.cpp" })
MakeProject("float_literals", { "./float_literals.cpp" })
MakeProject("integer_literals", { "./integer_literals.cpp" })
MakeProject("nested_structures", { "./nested_structures.cpp" })
MakeProject("nested_structures_2", { "./nested_structures_2.cpp" })
MakeProject("optional_arguments", { "./optional_arguments.cpp" })
MakeProject("integer_literals", { "./integer_literals.cpp" })
MakeProject("integer_literals", { "./integer_literals.cpp" })
MakeProject("integer_literals", { "./integer_literals.cpp" })
MakeProject("integer_literals", { "./integer_literals.cpp" })
MakeProject("option_delimiters", { "./option_delimiters.cpp" })
MakeProject("pair_argument", { "./pair_argument.cpp" })
MakeProject("positional_and_compound_arguments", { "./positional_and_compound_arguments.cpp" })
MakeProject("positional_and_optional_arguments", { "./positional_and_optional_arguments.cpp" })
MakeProject("positional_arguments", { "./positional_arguments.cpp" })
MakeProject("printing_help", { "./printing_help.cpp" })
MakeProject("printing_help_custom", { "./printing_help_custom.cpp" })
MakeProject("remaining_arguments", { "./remaining_arguments.cpp" })
MakeProject("tuple_arguments", { "./tuple_arguments.cpp" })
MakeProject("ristricted_arguments", { "./ristricted.cpp" })
