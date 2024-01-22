#include "IOptimize.h"

#include <Windows.h>
#include <powersetting.h>
#include <powrprof.h>
#include <stdio.h>


void IOptimizeSetPowerScheme() {
	GUID* currentPowerScheme = NULLPTR;
	//GUID* newPowerScheme = NULLPTR;
	GUID subPowerScheme = GUID_PROCESSOR_SETTINGS_SUBGROUP;


	if (PowerGetActiveScheme(NULL, &currentPowerScheme) != ERROR_SUCCESS) {
		MessageBoxA(NULLPTR, "Failed to get current power scheme!", "Error!", MB_OK | MB_ICONERROR);
		return;
	}

	//if (PowerDuplicateScheme(NULL, currentPowerScheme, &newPowerScheme) != ERROR_SUCCESS) {
	//	MessageBoxA(NULLPTR, "Failed to duplicate power scheme!", "Error!", MB_OK | MB_ICONERROR);
	//	return;
	//}

	UCHAR buf[128] = { 0 };
	DWORD bufSize = 128;
	int result;
	for (int i = 0; PowerEnumerate(NULL, currentPowerScheme, &subPowerScheme, ACCESS_INDIVIDUAL_SETTING, i, buf, &bufSize) == 0; i++) {
		WCHAR friendlyName[1024] = { 0 };
		DWORD friendlyNameSize = 1024;

		result = PowerReadFriendlyName(NULL, currentPowerScheme, &subPowerScheme, (const GUID*)buf, (UCHAR*)friendlyName, &friendlyNameSize);

		// See https://www.youtube.com/watch?v=n9xAG3nb9XQ&t=197s for more info.
		if (wcscmp(friendlyName, L"Processor performance time check interval") == '\0') {
			WCHAR info[512] = { 0 };
			int infoSize = 512;
			result = StringFromGUID2((const GUID *const)buf, info, infoSize);

			int processorPerformanceTimeCheckIntervalValue = 5000;

			// result = PowerWriteValueMax(NULL, &subPowerScheme, (const GUID*)buf, processorPerformanceTimeCheckIntervalValue);

			result = PowerWriteACValueIndex(NULL, currentPowerScheme, &subPowerScheme, (const GUID*)buf, processorPerformanceTimeCheckIntervalValue);
			wprintf(L"%s GUID: %s\nValue: %i", friendlyName, info, processorPerformanceTimeCheckIntervalValue);
		}
	}

	PowerSetActiveScheme(NULL, currentPowerScheme);

	LocalFree(currentPowerScheme);
}