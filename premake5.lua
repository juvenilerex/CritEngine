workspace "CritEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"ReleaseDev",
		"Release"
	}

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CritEngine"
	location "CritEngine"
	kind "SharedLib"
	language "C++"
	
	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDirectory .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	postbuildcommands 
	{ 
		"{MKDIR} ../bin/" .. outputDirectory .. "/Sandbox", -- Prevents the next command causing the dll being copied as a file named "Sandbox" in the parent directory.
		"{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDirectory .. "/Sandbox" 
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"ENGINE_BUILD_DLL",
			"CE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		symbols "On"

	filter "configurations:ReleaseDev"
		defines "CE_RELEASEDEV"
		optimize "On"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDirectory .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{wks.location}/CritEngine/src"
	}
	
	links 
	{
		"CritEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"CE_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "CE_DEBUG"
		symbols "On"

	filter "configurations:ReleaseDev"
		defines "CE_RELEASEDEV"
		optimize "On"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "On"