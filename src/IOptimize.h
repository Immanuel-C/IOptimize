#pragma once

#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* Creating new functionality for IOptimize.
* 
* Prerequisites:
*	- If you don't know how C/C++ calling conventions work read https://learn.microsoft.com/en-us/cpp/cpp/argument-passing-and-naming-conventions?view=msvc-170.
* 
* Creating new functions:
*	- All functions that will be exposed to C# must have the __IOPTIMIZE_STDCALL calling convention and must not have variable arguments.
*	- All functions with variable arguments must use the __IOPTIMIZE_CDECL calling convention.
*	- All functions that only have parameters integral types or pointers must use calling convention __IOPTIMIZE_VECTORCALL.
*	- All functions that do not have variable arguments and are not exposed to C# must use __IOPTIMIZE_FASTCALL.
*	- All functions must have tests in the IOptimizeTest project. See Test.c to see how to properly create and call functions.
*/

/**
* Set the current power scheme setting(s).
* @param powerOptions A pointer to an array of IOptimizePowerOptions that contains the values to set.
* @param powerOptionsSize A size_t that is set to how much elements powerOptions has.
* @param powerSchemeSubGroup a IOptimizePowerSchemeSubGroup that represents the GUID of a subgroup in the current power scheme. This function will only check for settings in this sub group.
*/
IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeSetPowerScheme(IOptimizePowerOption* powerOptions, size_t powerOptionsSize, IOptimizePowerSchemeSubGroup powerSchemeSubGroup);
/**
* Get the current power scheme setting(s)
* @param powerOptions A pointer to an array of IOptimizePowerOptions. IOptimizePowerOptions::name must be set (e.g. Processor performance time check interval) and this function will set the values to what the Windows api returns.
* @param powerOptionsSize A size_t that is set to how much elements powerOptions has.
* @param powerSchemeSubGroup a IOptimizePowerSchemeSubGroup that represents the GUID of a subgroup in the current power scheme. This function will only check for settings in this sub group.
*/
IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeQueryPowerSettingsValues(IOptimizePowerOption* powerOptions, size_t powerOptionsSize, IOptimizePowerSchemeSubGroup powerSchemeSubGroup);
/**
* Set graphics cards message signal interrupt mode.
* 
* @param msi an IOptimizeBool that must be either IOptimizeBoolValues::IOPTIMIZE_TRUE or IOptimizeBoolValues::IOPTIMIZE_FALSE. IOptimizeBoolValues::IOPTIMIZE_TRUE enables msi mode and IOptimizeBoolValues::IOPTIMIZE_FALSE disables it.
*/
IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeSetGpuMsiMode(IOptimizeBool msi);
/**
* Set graphics cards message signal interrupt mode.
*
* @param width an int that is the new width of the display
* @param height an int that is the new height of the display
*/
IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeSetDisplayResolution(int width, int height);
/**
* Set IOptimize registry tweaks.
*
* @param optimizeType is an IOptimizeTypeFlags. If this bitfield contains IOptimizeTypeFlags::IOPTIMIZE_TYPE_FORTNITE or IOptimizeTypeFlags::IOPTIMIZE_TYPE_APEX_LEGENDS than it also must contain IOptimizeTypeFlags::IOPTIMIZE_TYPE_FPS
*/
IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);
/**
* Set the windows global timer resolution. This only works on Windows 11 and with IOptimizeSetRegistryTweaks() with IOptimizeTypeFlags::IOPTIMIZE_TYPE_LATENCY tweak enabled.
*
* @param resolutionMS a uint32_t that contains the resolution in miliseconds * 10000. 
*/
IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeSetTimerResolution(uint32_t resolutionMS);
/**
* Get the windows global timer resolution, with the min and max timer resolution. 
* 
* @return all values are in miliseconds * 10000 format.
*/
IOPTIMIZE_API IOptimizeTimerResolutionValues __IOPTIMIZE_STDCALL IOptimizeQueryTimerResolution();
/**
* Micro adjust timer resolution as these could give more better results than setting the timer resolution to the max.
*
* Credit to amitxv for implementation.
* https://github.com/amitxv/TimerResolution/blob/main/micro-adjust-benchmark.ps1
* https://github.com/amitxv/PC-Tuning/blob/main/docs/research.md#micro-adjusting-timer-resolution-for-higher-precision
* https://github.com/amitxv/TimerResolution/blob/main/MeasureSleep/MeasureSleep/MeasureSleep.cpp
* 
* @param start a uint32_t that is the start of the micro adjusting. Must be greater or equal to the max timer resolution returned by IOptimizeQueryTimerResolution().
* @param end a uint32_t that is the end of the micro adjusting. Must be less than or equal to the min timer resolution returned by IOptimizeQueryTimerResolution().
* @param increment a uint32_t that is added to start until it reaches end. Each time this is added the function will benchmark the resolution.
* @param samples a uint32_t that defines how many times we should benchmark each resolution. The higher the samples the longer the benchmark will be but the more accurate the results will be.
* 
* @return the best timer resolution.
*/
IOPTIMIZE_API uint32_t __IOPTIMIZE_STDCALL IOptimizeMicroAdjustTimerResolution(uint32_t start, uint32_t end, uint32_t increment, uint32_t samples);

#ifdef __cplusplus
}
#endif
