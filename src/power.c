#include "IOptimize.h"

#include <Windows.h>
#include <powersetting.h>
#include <powrprof.h>
#include <stdio.h>

void __IOPTIMIZE_VECTORCALL _IOptimizeRWPowerScheme(IOptimizePowerOption* powerOptions, size_t powerOptionsSize, IOptimizePowerSchemeSubGroup powerSchemeSubGroup, IOptimizeBool write) {
	IOPTIMIZE_ASSERT(powerOptions != IOPTIMIZE_NULLPTR, "IOptimizeSetPowerScheme() param powerOptions must not be null.");
	IOPTIMIZE_ASSERT(powerOptionsSize != 0, "IOptimizeSetPowerScheme() param powerOptionsSize must not be 0.");

	GUID* currentPowerScheme = IOPTIMIZE_NULLPTR;
	GUID subPowerScheme = { 0 };
	IOptimizeConvertIOptimizePowerSchemeSubGroupToGUID(powerSchemeSubGroup, &subPowerScheme);

	if (PowerGetActiveScheme(NULL, &currentPowerScheme) != ERROR_SUCCESS) {
		MessageBoxA(IOPTIMIZE_NULLPTR, "Failed to get current power scheme!", "Error!", MB_OK | MB_ICONERROR);
		return;
	}

	UCHAR buf[256] = { 0 };
	DWORD bufSize = 256;
	uint32_t result;
	for (uint32_t i = 0; PowerEnumerate(IOPTIMIZE_NULLPTR, currentPowerScheme, &subPowerScheme, ACCESS_INDIVIDUAL_SETTING, i, buf, &bufSize) == 0; i++)
		for (size_t y = 0; y < powerOptionsSize; y++) {
			WCHAR friendlyName[1024] = { 0 };
			DWORD friendlyNameSize = 1024;

			result = PowerReadFriendlyName(NULL, currentPowerScheme, &subPowerScheme, (const GUID*)buf, (UCHAR*)friendlyName, &friendlyNameSize);
			IOPTIMIZE_ASSERT(result == ERROR_SUCCESS, "PowerReadFriendlyName() failed!");

			if (wcscmp(friendlyName, powerOptions[y].name) == '\0') {
				WCHAR info[512] = { 0 };
				int infoSize = 512;
				result = StringFromGUID2((const GUID* const)buf, info, infoSize);
				IOPTIMIZE_ASSERT(result != 0, "StringFromGUID2() failed!");


				if (powerOptions[y].maxValue != IOPTIMIZE_DEFUALT && write == IOPTIMIZE_TRUE) {
					if (write == IOPTIMIZE_TRUE) {
						result = PowerWriteValueMax(NULL, &subPowerScheme, (const GUID*)buf, (DWORD)powerOptions[y].maxValue);
						IOPTIMIZE_ASSERT(result == ERROR_SUCCESS, "PowerWriteValueMax() failed!");
					}

				}
				else {
					result = PowerReadValueMax(NULL, &subPowerScheme, (const GUID*)buf, (DWORD*)&powerOptions[y].maxValue);
					IOPTIMIZE_ASSERT(result == ERROR_SUCCESS, "PowerReadValueMax() failed!");
				}

				if (write == IOPTIMIZE_TRUE) {
					result = PowerWriteACValueIndex(NULL, currentPowerScheme, &subPowerScheme, (const GUID*)buf, (DWORD)powerOptions[y].value);
					IOPTIMIZE_ASSERT(result == ERROR_SUCCESS, "PowerWriteACValueIndex() failed!");
					wprintf(L"%s GUID: %s\nValue: %u\n", friendlyName, info, powerOptions[y].value);
				}
				else {
					PowerReadACValueIndex(NULL, currentPowerScheme, &subPowerScheme, (const GUID*)buf, (DWORD*)&powerOptions[y].value);
					IOPTIMIZE_ASSERT(result == ERROR_SUCCESS, "PowerWriteACValueIndex() failed!");
				}
			}
		}

	PowerSetActiveScheme(NULL, currentPowerScheme);

	LocalFree(currentPowerScheme);
}

void __IOPTIMIZE_STDCALL IOptimizeSetPowerScheme(IOptimizePowerOption* powerOptions, size_t powerOptionsSize, IOptimizePowerSchemeSubGroup powerSchemeSubGroup) {
	_IOptimizeRWPowerScheme(powerOptions, powerOptionsSize, powerSchemeSubGroup, IOPTIMIZE_TRUE);
}

void __IOPTIMIZE_STDCALL IOptimizeQueryPowerSettingsValues(IOptimizePowerOption* powerOptions, size_t powerOptionsSize, IOptimizePowerSchemeSubGroup powerSchemeSubGroup) {
	_IOptimizeRWPowerScheme(powerOptions, powerOptionsSize, powerSchemeSubGroup, IOPTIMIZE_FALSE);
}
