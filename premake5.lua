-- Premake LearnVulkan
workspace "LearnVulkan"
    architecture "x86_64"
    startproject "LearnVulkan"

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
IncludeDir["glad"] = "LearnVulkan/External/glad/include"
IncludeDir["glfw"] = "LearnVulkan/External/glfw/include"
IncludeDir["glm"] = "LearnVulkan/External/glm"

group "Dependencies"
    include "LearnVulkan/External/glad"
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
        "%{prj.name}/Source/**.c",
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
        "%{IncludeDir.glad}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "glad",
        "glfw"
    }


    filter "system:windows"
        systemversion "latest"
        characterset ("MBCS")

    filter "configurations:Debug"
        defines "LEARN_VK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LEARN_VK_RELEASE"
        runtime "Release"
        optimize "on"
