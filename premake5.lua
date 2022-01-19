-- Premake LearnVulkan
workspace "LearnVulkan"
    architecture "x86_64"
    startproject "Editor"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"] = "LearnVulkan/External/glfw/include"
IncludeDir["glm"] = "LearnVulkan/External/glm"

group "Dependencies"
    include "LearnVulkan/External/glfw"
group ""

--LearnVulkan
project "LearnVulkan"
    location "LearnVulkan"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Precompiled.h"
    pchsource "LearnVulkan/Source/Precompiled.cpp"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.hpp",
        "%{prj.name}/Source/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "NOMINMAX",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{IncludeDir.glm}",
        "%{IncludeDir.glfw}",
    }

    links
    {
        "glfw",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "LEARN_VK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LEARN_VK_RELEASE"
        runtime "Release"
        optimize "on"
