#pragma once

#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif


IOPTIMIZE_API void IOptimizeSetPowerScheme();
IOPTIMIZE_API void IOptimizeSetGpuMsiMode(int msi);
IOPTIMIZE_API void IOptimizeSetDisplayResolution(int width, int height);
IOPTIMIZE_API void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);
IOPTIMIZE_API void IOptimizeSetTimerResolution(uint32_t resolutionMS);
IOPTIMIZE_API struct IOptimizeTimerResolutionValues IOptimizeQueryTimerResolution();
IOPTIMIZE_API uint32_t IOptimizeMicroAdjustTimerResolution();

#ifdef __cplusplus
}
#endif
