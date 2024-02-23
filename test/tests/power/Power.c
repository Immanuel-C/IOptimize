#include "power.h"

void __IOPTIMIZE_STDCALL TestPower() {
	uint32_t setValue = 5000;
	uint32_t maxValue = 5100;

	// See https://www.youtube.com/watch?v=n9xAG3nb9XQ&t=197s for more info.
	IOptimizePowerOption processorPerformenceTimeCheckInterval = { 0 };
	processorPerformenceTimeCheckInterval.name = L"Processor performance time check interval";
	processorPerformenceTimeCheckInterval.value = setValue;
	// Optional
	processorPerformenceTimeCheckInterval.maxValue = maxValue;

	IOptimizeSetPowerScheme(&processorPerformenceTimeCheckInterval, 1, IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PROCESSOR_POWER_MANAGMENT);

	processorPerformenceTimeCheckInterval.value = 0;
	processorPerformenceTimeCheckInterval.maxValue = 0;

	IOptimizeQueryPowerSettingsValues(&processorPerformenceTimeCheckInterval, 1, IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PROCESSOR_POWER_MANAGMENT);

	IOPTIMIZE_ASSERT(processorPerformenceTimeCheckInterval.value == setValue, "IOptimizeQueryPowerSettingsValues() returned a different value for Processor performance time check interval than what IOptimizeSetPowerScheme set it to.");
	IOPTIMIZE_ASSERT(processorPerformenceTimeCheckInterval.maxValue == maxValue, "IOptimizeQueryPowerSettingsValues() returned a different max value for Processor performance time check interval than what IOptimizeSetPowerScheme set it to.");

	IOptimizeLog("Processor performance time check interval:\n\tvalue: %u\n\tmax value: %u\n", processorPerformenceTimeCheckInterval.value, processorPerformenceTimeCheckInterval.maxValue);

	processorPerformenceTimeCheckInterval.value = 15;
	
	// Set it back to default
	// IOptimizeSetPowerScheme(&processorPerformenceTimeCheckInterval, 1, IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PROCESSOR_POWER_MANAGMENT);
}