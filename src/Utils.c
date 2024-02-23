#include "Utils.h"

#include <stdarg.h>
#include <stdio.h>

void __IOPTIMIZE_CDECL IOptimizeLog(const char* fmt, ...) {
	va_list vaArgs;

	va_start(vaArgs, fmt);

	vfprintf(stdout, fmt, vaArgs);

	va_end(vaArgs);
}

void __IOPTIMIZE_CDECL IOptimizeLogErr(const char* fmt, ...) {
	va_list vaArgs;

	va_start(vaArgs, fmt);

	vfprintf(stderr, fmt, vaArgs);

	va_end(vaArgs);

}

IOPTIMIZE_API void __IOPTIMIZE_CDECL IOptimizeSprintf(char* buffer, size_t bufferSize, const char* fmt, ...) {
	va_list vaArgs;

	va_start(vaArgs, fmt);

	#ifdef _MSC_VER

	vsprintf_s(buffer, bufferSize, fmt, vaArgs);

	#else
	
	vsprintf(buffer, fmt, vaArgs);

	#endif

	va_end(vaArgs);
}


IOPTIMIZE_API void* __IOPTIMIZE_STDCALL IOptimizeLoadFunction(wchar_t* libraryName, wchar_t* functionName) {
	HINSTANCE handle = LoadLibraryW(libraryName);

	if (handle == IOPTIMIZE_NULLPTR) return IOPTIMIZE_NULLPTR;

	void* fun = GetProcAddress(handle, functionName);

	return fun;
}

static void __IOPTIMIZE_STDCALL IOptimizeGUIDFromString(wchar_t* strGuid, GUID* guid) {
	IOPTIMIZE_ASSERT(guid != IOPTIMIZE_NULLPTR, "IOptimizeGUIDFromString param guid must not be null.");
	IOPTIMIZE_ASSERT(strGuid != IOPTIMIZE_NULLPTR, "IOptimizeGUIDFromString param strGuid must not be null.");

	HRESULT result = CLSIDFromString(strGuid, guid);
	
	IOPTIMIZE_ASSERT(result != CO_E_CLASSSTRING, "CLSIDFromString() failed with error code CO_E_CLASSSTRING. IOptimizeGUIDFromString() param strGuid is improperly formated.");
}

IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeConvertIOptimizePowerSchemeSubGroupToGUID(IOptimizePowerSchemeSubGroup subgroup, GUID* guid) {
	switch (subgroup)
	{
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PROCESSOR_POWER_MANAGMENT:
			(*guid) = GUID_PROCESSOR_SETTINGS_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_HARD_DISK:
			(*guid) = GUID_DISK_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_INTERNET_EXPLORER:
			// Subgroups that are not defined in the winnt.h header file we have to manually create GUID's.
			IOptimizeGUIDFromString(L"{02f815b5-a5cf-4c84-bf20-649d1f75d3d8}", guid);
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_DESKTOP_BACKGROUND_SETTINGS:
			IOptimizeGUIDFromString(L"{0d7dbae2-4294-402a-ba8e-26777e8488cd}", guid);
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_WIRELESS_ADAPTOR_SETTINGS:
			IOptimizeGUIDFromString(L"{19cbb8fa-5279-450e-9fac-8a3d5fedd0c1}", guid);
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_SLEEP:
			(*guid) = GUID_SLEEP_SUBGROUP;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_USB_SETTINGS:
			IOptimizeGUIDFromString(L"{238c9fa8-0aad-41ed-83f4-97be242c8f20}", guid);
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_IDLE_RESILIENCY:
			(*guid) = GUID_IDLE_RESILIENCY_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_INTERRUPT_STEERING_SETTINGS:
			(*guid) = GUID_INTSTEER_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_POWER_BUTTONS_AND_LID:
			(*guid) = GUID_SYSTEM_BUTTON_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PCI_EXPRESS:
			(*guid) = GUID_PCIEXPRESS_SETTINGS_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_GRAPHICS_SETTINGS:
			(*guid) = GUID_GRAPHICS_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_DISPLAY:
			(*guid) = GUID_VIDEO_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PRESENCE_AWARE_POWER_BEHAVIOR:
			(*guid) = GUID_ADAPTIVE_POWER_BEHAVIOR_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_MULTIMEDIA_SETTINGS:
			IOptimizeGUIDFromString(L"{9596fb26-9850-41fd-ac3e-f7c3c00afd4b}", guid);
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_ENERGY_SAVER_SETTINGS:
			(*guid) = GUID_ENERGY_SAVER_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_BATTERY:
			(*guid) = GUID_BATTERY_SUBGROUP;
			break;
		case IOPTIMIZE_POWER_SCHEME_SUB_GROUP_NONE:
			(*guid) = NO_SUBGROUP_GUID;
			break;
	default:
		guid = IOPTIMIZE_NULLPTR;
		break;
	}
}
