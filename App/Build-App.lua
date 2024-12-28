project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "include/**.hpp", "src/**.cpp" }

   includedirs {
      "include",

	  -- Include Core
	  "../Raycaster2D"
   }

   libdirs
   {
      "../Raycaster2D/3rd Party/SFML/build/lib"
   }

   links {
      "Raycaster2D",
      "opengl32.lib",
      "winmm.lib",
      "gdi32.lib"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   defines { "SFML_STATIC" }

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"
       links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d", "freetyped" }

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"
       links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s", "freetype" }

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"