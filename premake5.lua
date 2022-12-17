workspace "BagelEngine"
	architecture "x64"
	startproject "Sandbox"


	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "BagelEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "BagelEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "BagelEngine/vendor/imgui"
IncludeDir["glm"] = "BagelEngine/vendor/glm"
IncludeDir["stb_image"] = "BagelEngine/vendor/stb_image"




group "Dependancies"
	include "BagelEngine/vendor/GLFW"
	include "BagelEngine/vendor/Glad"
	include "BagelEngine/vendor/imgui"

group ""

project "BagelEngine"
	location "BagelEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bgpch.h"
	pchsource "BagelEngine/src/bgpch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.GLFW}", 
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS",
			"BG_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug" 
		defines "BG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist" 
		defines "BG_DIST"
		runtime "Release"
		optimize "on"

	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"BagelEngine/vendor/spdlog/include",
		"BagelEngine/src",
		"BagelEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links {
		"BagelEngine"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug" 
		defines "BG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist" 
		defines "BG_DIST"
		runtime "Release"
		optimize "on"
