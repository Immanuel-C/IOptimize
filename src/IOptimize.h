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
IOPTIMIZE_API IOptimizeTimerResolutionValues IOptimizeQueryTimerResolution();
IOPTIMIZE_API uint32_t IOptimizeMicroAdjustTimerResolution(uint32_t start, uint32_t end, uint32_t increment, uint32_t samples, uint32_t sleepTimeBetweenQueries);

#ifdef __cplusplus
}
#endif
