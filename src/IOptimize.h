#pragma once

#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* Set the Custom IOptimize Power Scheme.
*/
IOPTIMIZE_API void IOptimizeSetPowerScheme();
/**
* Set graphics cards message signal interrupt mode.
* 
* @param msi an int that must be either 1 or 0. 1 enables msi mode and 0 disables it.
*/
IOPTIMIZE_API void IOptimizeSetGpuMsiMode(int msi);
/**
* Set graphics cards message signal interrupt mode.
*
* @param width an int that is the new width of the display
* @param height an int that is the new height of the display
*/
IOPTIMIZE_API void IOptimizeSetDisplayResolution(int width, int height);
/**
* Set IOptimize registry tweaks.
*
* @param optimizeType is an IOptimizeTypeFlags. If this bitfield contains IOptimizeTypeFlags::IOPTIMIZE_TYPE_FORTNITE or IOptimizeTypeFlags::IOPTIMIZE_TYPE_APEX_LEGENDS than it also must contain IOptimizeTypeFlags::IOPTIMIZE_TYPE_FPS
*/
IOPTIMIZE_API void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);
/**
* Set the windows global timer resolution. This only works on Windows 11 and with IOptimizeSetRegistryTweaks() with IOptimizeTypeFlags::IOPTIMIZE_TYPE_LATENCY tweak enabled.
*
* @param resolutionMS a uint32_t that contains the resolution in miliseconds * 10000. 
*/
IOPTIMIZE_API void IOptimizeSetTimerResolution(uint32_t resolutionMS);
/**
* Get the windows global timer resolution, with the min and max timer resolution. 
* 
* @return all values are in miliseconds * 10000 format.
*/
IOPTIMIZE_API IOptimizeTimerResolutionValues IOptimizeQueryTimerResolution();
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
IOPTIMIZE_API uint32_t IOptimizeMicroAdjustTimerResolution(uint32_t start, uint32_t end, uint32_t increment, uint32_t samples);

#ifdef __cplusplus
}
#endif
