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
	runtime "Release"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}", 
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	pchheader "bgpch.h"
	pchsource "BagelEngine/src/bgpch.cpp"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"BG_BUILD_DLL",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
		}



	filter "configurations:Debug" 
		defines "BG_DEBUG"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist" 
		defines "BG_DIST"
		runtime "Release"
		buildoptions "/MD"
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
		"BagelEngine/src"
	}

	links {
		"BagelEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug" 
		defines "BG_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist" 
		defines "BG_DIST"
		buildoptions "/MD"
		optimize "On"
