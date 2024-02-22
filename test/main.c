#include <IOptimize.h>

#include <stdio.h>

int main() {
	// See https://www.youtube.com/watch?v=n9xAG3nb9XQ&t=197s for more info.
	IOptimizePowerOption processorPerformenceTimeCheckInterval = { 0 }; 
	processorPerformenceTimeCheckInterval.name = L"Processor performance time check interval";
	processorPerformenceTimeCheckInterval.value = 5000;
	processorPerformenceTimeCheckInterval.maxValue = IOPTIMIZE_DEFUALT;
	// processorPerformenceTimeCheckInterval.maxValue = 5000;

	IOptimizeSetPowerScheme(&processorPerformenceTimeCheckInterval, 1, GUID_PROCESSOR_SETTINGS_SUBGROUP);

	return 0;
}