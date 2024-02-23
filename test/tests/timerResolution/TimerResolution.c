#include "TimerResolution.h"

void __IOPTIMIZE_STDCALL TestTimerResolution() {
	uint32_t chosenTimerResolution = IOptimizeMicroAdjustTimerResolution(5000, 5100, 20, 20);

	IOPTIMIZE_ASSERT(chosenTimerResolution >= 5000, "IOptimizeMicroAdjustTimerResolution() must return a value higher than the max resolution");
	IOPTIMIZE_ASSERT(chosenTimerResolution <= 5100, "IOptimizeMicroAdjustTimerResolution() must return a value lower than the min resolution");

	IOptimizeSetTimerResolution(chosenTimerResolution);

	IOptimizeTimerResolutionValues values =  IOptimizeQueryTimerResolution();

	IOptimizeLog("Maximum resolution: %u, Minimum resolution: %u, Current resolution: %u\n", values.maxResolution, values.minResolution, values.currResolution);
}