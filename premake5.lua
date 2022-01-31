workspace "clox"
	configurations { "debug", "release" }

project "clox"
	kind "ConsoleApp"
	language "C"
	cdialect "C11"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj"
	warnings "Extra"

	files { "source/**.h", "source/**.c" }

	filter { "configurations:debug" }
		symbols "On"

	filter { "configurations:release" }
		optimize "On"