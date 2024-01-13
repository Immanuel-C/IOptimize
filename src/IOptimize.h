#pragma once

#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

IOPTIMIZE_API void IOptimizeSetGpuMsiMode(int msi);
IOPTIMIZE_API void IOptimizeSetDisplayResolution(int width, int height);
IOPTIMIZE_API void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);

#ifdef __cplusplus
}
#endif
