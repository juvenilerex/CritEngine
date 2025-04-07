local DependencyFetcher = require "dependency_fetcher" 
DependencyFetcher.Setup()  -- Check and fetch dependencies before anything else

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
	includedirs { "CritEngine/thirdparty/libs/glfw/include/", "CritEngine/thirdparty/libs/glad/include/", "CritEngine/thirdparty/libs/glm/", "CritEngine/thirdparty/libs/imgui/", "CritEngine/thirdparty/libs/imgui/examples"}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	links { "GLFW", "GLM", "GLAD", "ImGui" }

	postbuildcommands 
	{ 
		"{MKDIR} ../bin/" .. outputDirectory .. "/Sandbox", -- Prevents the next command causing the dll being copied as a file named "Sandbox" in the parent directory.
		"{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDirectory .. "/Sandbox" 
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
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

	include "CritEngine/thirdparty/libs/glfw.lua"
	include "CritEngine/thirdparty/libs/glad.lua"
	include "CritEngine/thirdparty/libs/glm.lua"
	include "CritEngine/thirdparty/libs/imgui.lua"

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
		"%{wks.location}/CritEngine/src",
		"CritEngine/thirdparty/libs/glm/"
	}
	
	links 
	{
		"CritEngine", "GLM"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
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
