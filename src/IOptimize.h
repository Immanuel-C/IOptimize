#pragma once

#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) int IOptimizeSetGpuMsiMode(int msi);
__declspec(dllexport) void IOptimizeSetDisplayResolution(int width, int height);
__declspec(dllexport) void IOptimizeSetRegistryTweaks(IOptimizeTypeFlags optimizeType);

#ifdef __cplusplus
}
#endif
