project "Raycaster2D"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {
        "include/**.hpp",
        "src/**.cpp",
        "**.lua",
        "**.md"
    }

    includedirs {
        "include",
        "3rd Party/SFML/include"
    }

    libdirs {
        "3rd Party/SFML/build/lib"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    defines { "SFML_STATIC" }

    filter "system:windows"
        systemversion "latest"
        defines { }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
        links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
        links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"