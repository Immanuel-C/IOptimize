#include <IOptimize.h>

#include <stdio.h>

int main() {
	IOptimizeSetTimerResolution(IOptimizeMicroAdjustTimerResolution(5000, 5400, 50, 10));

	while(IOPTIMIZE_TRUE);

	return 0;
}