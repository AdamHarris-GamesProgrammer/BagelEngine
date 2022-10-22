workspace "BagelEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "BagelEngine"
	location "BagelEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines {
			"BG_PLATFORM_WINDOWS",
			"BG_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")

		}


	filter "configurations:Debug" 
		defines "BG_DEBUG"
		symbols "On"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		optimize "On"

	filter "configurations:Dist" 
		defines "BG_DIST"
		optimize "On"

	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		}

	filter "configurations:Debug" 
		defines "BG_DEBUG"
		symbols "On"

	filter "configurations:Release" 
		defines "BG_RELEASE"
		optimize "On"

	filter "configurations:Dist" 
		defines "BG_DIST"
		optimize "On"
