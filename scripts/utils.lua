local OUTPUTDIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
local BUILDKIND = "ConsoleApp"
local LANGUAGE = "C++"
-- local OBJECTDIR = "../build/" .. "%{wks.name}/" .. OUTPUTDIR .. "/bin-int/"  .. "/%{prj.name}"
local OBJECTDIR = "../build/" .. "%{wks.name}/" .. "bin-int/"  .. "/%{prj.name}"
-- local TARGETDIR = "../build/" .. "%{wks.name}/"  .. OUTPUTDIR  .. "/bin/" .. "/%{prj.name}"
local TARGETDIR = "../build/" .. "%{wks.name}/"   .. "bin/" .. "/%{prj.name}"
local STANDARD = "C++17"

--- @class utils
--- @field makeProject fun(Name: string, Files: string[], IncludeDirs: string[], ObjectDir: string | nil, TargetDir: string | nil)
--- @field makeWorkspace fun(Name: string, BuildScriptLocation: string, Configs: string[], Platforms: string[], PreProcessorDefines: string[] | nil)
--- @field outputDir "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
--- @field objectDir string
--- @field targetDir string
--- @field BUILDKIND string
--- @field LANGUAGE string
--- @field STANDARD string
local M = {}

M.BUILDKIND = BUILDKIND
M.LANGUAGE = LANGUAGE
M.STANDARD = STANDARD
M.outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
M.objectDir = OBJECTDIR
M.targetDir = TARGETDIR

--- makes basic a premake project
--- if you need to add more options, configure the project manually
--- this is for basic scaffolding
function M.makeProject(Name, Files, IncludeDirs, ObjectDir, TargetDir)
	local target_dir = TargetDir or TARGETDIR
	local object_dir = ObjectDir or OBJECTDIR
	project(Name)
	kind(BUILDKIND)
	language(LANGUAGE)
	cppdialect(STANDARD)
	targetdir(target_dir)
	objdir(object_dir)
	includedirs(IncludeDirs)
	files(Files)
end

--- make a basic premake workspace
--- if you need to add more options, configure the project manually
--- this is for basic scaffolding
function M.makeWorkspace(Name, BuildScriptLocation, Configs, Platforms, PreProcessorDefines)
	workspace(Name)
	location(BuildScriptLocation)
	configurations(Configs)
	platforms(Platforms)

	if PreProcessorDefines ~= nil then
		defines(PreProcessorDefines)
	end

	filter("system:windows")
	system("windows")
	-- clangtidy("On")
	buildoptions({"/W4"})

	filter("system:linux", "action:gmake")
	buildoptions({ "-Wall", "-Wno-long-long", "-pedantic" })
end

return M
