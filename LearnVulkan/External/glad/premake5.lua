-- Premake glad
project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        --"include/glad/egl.h",
        "include/glad/gl.h",
        --"include/glad/gles1.h",
        --"include/glad/gles2.h",
        --"include/glad/glsc2.h",
        --"include/glad/glx.h",
        "include/glad/vulkan.h",
        --"include/glad/wgl.h",
        "include/KHR/khrplatform.h",
        --"include/EGL/eglplatform.h",
        "include/vk_platform.h",
		--"src/egl.c",
        "src/gl.c",
        --"src/gles1.c",
        --"src/gles2.c",
        --"src/glsc2.c",
        --"src/glx.c",
        "src/vulkan.c",
        --"src/wgl.c"
    }

    includedirs
    {
        "include"
    }

	defines
    {
        "WIN32"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
