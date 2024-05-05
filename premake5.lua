workspace "autoplan"
	configurations { "Debug", "Release" }
	platforms { "Static", "Dynamic" }

project "autoplan"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"

	files { "src/**.hpp", "src/**.cpp" }

	filter "platforms:Static"
		kind "StaticLib"
	filter "platforms:Dynamic"
		kind "SharedLib"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
