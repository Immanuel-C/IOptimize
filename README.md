# IOptimize
An application to optimize windows for more fps and reduced input latency. Using proven registry tweaks and more...

## Warning!

**The IOptimize alpha can damage your Windows install, always create a restore point before using the alpha. IOptimize was also only designed for Windows 10/11 and has not been test on other versions.**

## How to Build Project

### **IOptimize is Windows only! Only building on Visual Studio 2022 has been tested!**

To build IOptimize you need a compiler that supports C17, .Net Framework V4.7.2 and a WPF editor. A standard below both C17 and the .Net Framework V4.7.2 may work but has not been tested. Visual Studio 2022 with the "Desktop Devolpment With C++" and ".Net Desktop Devolpment" tools will provide you with all of these tools.

You will also need to install premake5 for the build system. Use premake5 to build to generate to whatever editor/compiler you would like to use. 

For Visual Studio 2022 you can then press start and the application will run!

## How to build The C API

To build the C API you need a compiler that supports C17. You must link against 
- User32
- Advapi32
- Cfgmgr32
- Setupapi
- PowrProf
- Ntdll
- Kernel32

To build as a shared library you must define `IOPTIMIZE_EXPORT_DLL` in the preprocess defines.

##

More info and docs for the C API will be provided as this application is being devolped!
