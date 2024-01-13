workspace "IOptimize"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    function defaultBuildCfg()
        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"
            runtime "Debug"
            optimize "Debug"

        filter "configurations:Release"
            defines { "NDEBUG" }
            symbols "Off"
            runtime "Release"
            optimize "Speed"
    end

    function defaultBuildLocation()
        targetdir ("bin/%{prj.name}/%{cfg.buildcfg}")
        objdir ("bin-int/%{prj.name}/%{cfg.buildcfg}")
    end

    startproject "WpfFrontend"

    project "IOptimizedWpfFrontend"
        targetname "WPFFrontend"
        location "wpfFrontend/"
        language "C#"
        flags { "WPF" }
        kind "ConsoleApp"

        files {
            "**.xaml",
            "**.xaml.cs",
            "%{prj.location}/Properties/Resources.Designer.cs",
            "**.cs",
            "**.config",
            "**.settings",
            "**.resx",
        }

        links {
            "Microsoft.Csharp",
            "PresentationCore",
            "PresentationFramework",
            "System",
            "System.Core",
            "System.Data",
            "System.Data.DataSetExtensions",
            "System.Xaml",
            "System.Xml",
            "System.Xml.Linq",
            "WindowsBase",
            "IOptimize",
        }

        

        defaultBuildLocation()
        defaultBuildCfg()
    
        project "IOptimize"
            location "src"
            kind "SharedLib"
            language "C"
            cdialect "C17"

            files {"%{prj.location}/**.c", "%{prj.location}/**.h"}

            includedirs { "src" }

            links { "User32", "Advapi32", "Cfgmgr32", "Setupapi" } 

            postbuildcommands { "copy ..\\bin\\%{prj.name}\\%{cfg.buildcfg}\\IOptimize.dll ..\\bin\\IOptimizedWpfFrontend\\%{cfg.buildcfg}" }

            defaultBuildLocation()

            defines { "IOPTIMIZE_EXPORT_DLL" }

            filter "configurations:Debug"
               debuggertype "Mixed"

            defaultBuildCfg()

