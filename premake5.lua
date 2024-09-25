-- premake5.lua

workspace "structopt_examples"
    configurations { "Debug", "Release" }
    platforms { "x64" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "demo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/demo.cpp"}

project "compound_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/compound_arguments.cpp"}

project "double_dash_argument"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/double_dash_argument.cpp"}

project "enum_class_argument"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/enum_class_argument.cpp"}

project "flag_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/flag_arguments.cpp"}

project "float_literals"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/float_literals.cpp"}

project "integer_literals"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/integer_literals.cpp"}

project "nested_structures"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/nested_structures.cpp"}

project "nested_structures_2"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/nested_structures_2.cpp"}

project "optional_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/optional_arguments.cpp"}

project "option_delimiters"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/option_delimiters.cpp"}

project "pair_argument"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/pair_argument.cpp"}

project "positional_and_compound_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/positional_and_compound_arguments.cpp"}

project "positional_and_optional_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/positional_and_optional_arguments.cpp"}

project "positional_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/positional_arguments.cpp"}

project "printing_help"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/printing_help.cpp"}

project "printing_help_custom"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/printing_help_custom.cpp"}

project "remaining_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/remaining_arguments.cpp"}

project "tuple_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/tuple_arguments.cpp"}

project "ristricted_arguments"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs { "./src" }
    files { "./examples/ristricted.cpp"}