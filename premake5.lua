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

    startproject "IOptimizeWpfFrontend"

    project "IOptimizeWpfFrontend"
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
            "**.manifest"
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

        project "IOptimizeTest"
            location "test"
            kind "ConsoleApp"
            language "C"
            cdialect "C17"

            files {"%{prj.location}/**.c", "%{prj.location}/**.h"}

            includedirs { "src", "%{prj.location}" }

            links { "User32", "Advapi32", "Cfgmgr32", "Setupapi", "PowrProf", "Ntdll", "IOptimize", "Kernel32" } 

            defaultBuildLocation()

            defaultBuildCfg()

        project "IOptimize"
            location "src"
            kind "SharedLib"
            language "C"
            cdialect "C17"

            files {
                "%{prj.location}/**.c", 
                "%{prj.location}/**.h",
            }

            removefiles {
                "**.g.cs",
                "**.g.i.cs",
                ".NETFramework,Version=v4.7.2.AssemblyAttributes.cs",
                ".NETFramework,Version=v4.8.0.AssemblyAttributes.cs",
            }

            includedirs { "src" }

            links { "User32", "Advapi32", "Cfgmgr32", "Setupapi", "PowrProf", "Ntdll", "Kernel32" } 

            postbuildcommands { "copy ..\\bin\\%{prj.name}\\%{cfg.buildcfg}\\IOptimize.dll ..\\bin\\IOptimizeWpfFrontend\\%{cfg.buildcfg}" }
            postbuildcommands { "copy ..\\bin\\%{prj.name}\\%{cfg.buildcfg}\\IOptimize.dll ..\\bin\\IOptimizeTest\\%{cfg.buildcfg}" }

            defines { "IOPTIMIZE_EXPORT_DLL" }

            targetname "IOptimize"
            
            defaultBuildLocation()

            filter "configurations:Debug"
               debuggertype "Mixed"

            defaultBuildCfg()


