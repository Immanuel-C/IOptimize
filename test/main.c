#include <IOptimize.h>

int main() {
	IOptimizeSetTimerResolution(IOptimizeMicroAdjustTimerResolution(5000, 5380, 20, 10, 1));
	for (;;);

	return 0;
}