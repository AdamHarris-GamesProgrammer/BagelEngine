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

group "Dependancies"
	include "BagelEngine/vendor/GLFW"
	include "BagelEngine/vendor/Glad"
	include "BagelEngine/vendor/imgui"

group ""

project "BagelEngine"
	location "BagelEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bgpch.h"
	pchsource "BagelEngine/src/bgpch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
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
		cppdialect "C++17"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS",
			"BG_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug" 
		defines "BG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist" 
		defines "BG_DIST"
		runtime "Release"
		optimize "On"

	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		cppdialect "C++17"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug" 
		defines "BG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist" 
		defines "BG_DIST"
		runtime "Release"
		optimize "On"
