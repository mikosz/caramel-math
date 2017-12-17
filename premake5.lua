structure = require "structure"

include "premake5.cfg.lua"
include "googletest.lua"

ROOT_DIR = path.getabsolute(".")

location "build"	

workspace "caramel-math"

	configurations { "DebugStatic", "DebugMemcheckStatic", "ProfileBuildTimes", "ReleaseStatic" }
	
	platforms { "Win64" }
	
	language "C++"

	-- Configuration specific settings
	
	filter "configurations:Debug*"
		symbols "On"
		
	filter "configurations:DebugMemcheckStatic"
		buildoptions { "/Ob0" } -- disable inline expansion
		flags { "NoFramePointer" }
	
	filter "configurations:ProfileBuildTimes"
		buildoptions { "/Bt+ /showIncludes /nologo- /FC" }
	
	filter "configurations:Release*"
		defines { "NDEBUG" }
		optimize "On"
	
	-- Platform specific settings
	
	filter "platforms:Win32"
		system "Windows"
		architecture "x32"

	filter "platforms:Win64"
		system "Windows"
		architecture "x64"
		
	filter {}
	
	-- Warnings
	
	warnings "Extra"
	flags { "FatalWarnings" }
	
	flags { "MultiProcessorCompile" }
	
	filter "action:vs*"
		defines { "_SCL_SECURE_NO_WARNINGS" }
		buildoptions { "/std:c++latest" }
		defines { "GTEST_LANG_CXX11=1" } -- otherwise googletest fails to compile with c++latest
	filter {}

	structure.library_project("caramel-math", function()
			add_googletest_files()
		end
		)

	structure.create_build_all_project()
	structure.create_install_project()
	structure.create_run_tests_project()
	