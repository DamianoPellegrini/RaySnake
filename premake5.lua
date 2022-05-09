
workspace "RaySnake"
    architecture "x86_64"
    configurations { "Debug", "Release" }

    libs = {}
    libs["raylib"] = "%{wks.location}/libs/raylib"

    libdirs {
        "%{libs.raylib}/lib",
        "%{libs.raylib}/bin"
    }

intdir = "%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"
outdir = "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.architecture}/%{prj.name}"

newoption {
    trigger     = "raylibdyn",
    description = "Use raylib dynamic lib",
}

project "Snake"
    location "Snake"
    language "C++"
    cppdialect "C++17"
    kind "ConsoleApp"

    pchheader "pch.h"
    pchsource "%{prj.location}/src/pch.cpp"

    objdir ( intdir )
    targetdir ( outdir )

    files {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.rc",
        "%{prj.location}/resources/**.*"
    }

    includedirs {
        "%{prj.location}/src/",
        "%{libs.raylib}/include"
    }

    if _OPTIONS["raylibdyn"] then
        print("Going dynamic")
        links {
            "raylib"
        }
        postbuildcommands "{COPY} %{libs.raylib}/bin/raylib.dll %{outdir}"
    else
        print("Going static")
        links {
            "raylib_static"
        }
    end

    postbuildcommands "{COPY} %{prj.location}/resources/ %{outdir}/resources/"

    filter "Debug"
        defines { "DEBUG" }
        symbols "on"
    
    filter "Release"
        defines { "NDEBUG" }
        optimize "on"
    
    filter {} --Deselect filter