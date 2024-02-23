#pragma once

#include <stdint.h>
#include <Windows.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_WIN64)
#error IOptimize only supports 86_64x Windows!
#endif

#ifdef IOPTIMIZE_EXPORT_DLL
	#define IOPTIMIZE_API __declspec(dllexport)
#else 
	#define IOPTIMIZE_API __declspec(dllimport)
#endif


#define IOPTIMIZE_NULLPTR (void*)0

#define IOPTIMIZE_DEFUALT 0


/**
* Do not use __IOPTIMIZE_FASTCALL or __IOPTIMIZE_VECTORCALL for functions that are to be imported into C#. Use __IOPTIMIZE_STDCALL or __IOPTIMIZE_CDECL.
*/
#if defined(_MSC_VER) || defined(__clang__)

#define __IOPTIMIZE_CDECL __cdecl
#define __IOPTIMIZE_STDCALL __stdcall
#define __IOPTIMIZE_FASTCALL __fastcall
#define __IOPTIMIZE_VECTORCALL __vectorcall

#elif defined(__MINGW32__) || defined(__GNUC__) 

#define __IOPTIMIZE_CDECL __attribute__((cdecl))
#define __IOPTIMIZE_STDCALL __attribute__((stdcall))
#define __IOPTIMIZE_FASTCALL __attribute__((fastcall))
#define __IOPTIMIZE_VECTORCALL __IOPTIMIZE_FASTCALL

#endif

/**
* Represents a 2 floats.
* width and x are in a union
* height and y are in a union
*/
typedef struct IOptimizeVector2_T {
	union { float width, x;  };
	union { float height, y; };
} IOptimizeVector2;

enum IOptimizeTypeFlagBits {
	IOPTIMIZE_TYPE_NONE = 0x0000000ui64,
	IOPTIMIZE_TYPE_REVERT = 0x00000001ui64,
	IOPTIMIZE_TYPE_FPS = 0x00000002ui64,
	IOPTIMIZE_TYPE_LATENCY = 0x00000004ui64,
	/// Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
	IOPTIMIZE_TYPE_FORTNITE = 0x00000008ui64,
	/// Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
	IOPTIMIZE_TYPE_APEX_LEGENDS = 0x00000010ui64,
};

/**
* Represents the type of predefined optimization to be used.
*/
typedef enum IOptimizeTypeFlagBits IOptimizeTypeFlags;

/**
* The return values of NtQueryTimerResolution().
*/
typedef struct IOptimizeTimerResolutionValues_T {

	uint32_t 
		/// The maximun timer resolution.
		maxResolution, 
		/// The minimun timer resolution.
		minResolution, 
		/// The current timer resolution.
		currResolution;
} IOptimizeTimerResolutionValues;

enum IOptimizeBoolValues {
	IOPTIMIZE_FALSE = 0x0ui8,
	IOPTIMIZE_TRUE = 0x1ui8,
};

/**
* Represents a boolean type
*/
typedef enum IOptimizeBoolValues IOptimizeBool;

/**
* Represents an windows advanced power option.
*/
typedef struct IOptimizePowerOption_T {
	/// The name of the power option.
	wchar_t* name;
	///  The new value of the power option.
	uint32_t value;
	/// The new max value of the power option. Set to IOPTIMIZE_DEFUALT or 0 to keep the default value.
	uint32_t maxValue;

} IOptimizePowerOption;


typedef enum IOptimizePowerSchemeSubGroup_T {
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PROCESSOR_POWER_MANAGMENT,		// GUID_PROCESSOR_SETTINGS_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_HARD_DISK,						// GUID_DISK_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_INTERNET_EXPLORER,				// {02f815b5-a5cf-4c84-bf20-649d1f75d3d8}
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_DESKTOP_BACKGROUND_SETTINGS,	// {0d7dbae2-4294-402a-ba8e-26777e8488cd}
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_WIRELESS_ADAPTOR_SETTINGS,		// {19cbb8fa-5279-450e-9fac-8a3d5fedd0c1}
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_SLEEP,							// GUID_SLEEP_SUBGROUP 
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_USB_SETTINGS,					// {238c9fa8-0aad-41ed-83f4-97be242c8f20}
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_IDLE_RESILIENCY,				// GUID_IDLE_RESILIENCY_SUBGROUP 
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_INTERRUPT_STEERING_SETTINGS,	// GUID_INTSTEER_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_POWER_BUTTONS_AND_LID,			// GUID_SYSTEM_BUTTON_SUBGROUP 
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PCI_EXPRESS,					// GUID_PCIEXPRESS_SETTINGS_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_GRAPHICS_SETTINGS,				// GUID_GRAPHICS_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_DISPLAY,						// GUID_VIDEO_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_PRESENCE_AWARE_POWER_BEHAVIOR, // GUID_ADAPTIVE_POWER_BEHAVIOR_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_MULTIMEDIA_SETTINGS,			// {9596fb26-9850-41fd-ac3e-f7c3c00afd4b}
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_ENERGY_SAVER_SETTINGS,			// GUID_ENERGY_SAVER_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_BATTERY,						// GUID_BATTERY_SUBGROUP
	IOPTIMIZE_POWER_SCHEME_SUB_GROUP_NONE,							// NO_SUBGROUP_GUID

/**
* Gives a name for nameless sub groups.
*/
} IOptimizePowerSchemeSubGroup;

IOPTIMIZE_API void __IOPTIMIZE_CDECL IOptimizeLog(const char* fmt, ...);
IOPTIMIZE_API void __IOPTIMIZE_CDECL IOptimizeLogErr(const char* fmt, ...);

IOPTIMIZE_API void __IOPTIMIZE_CDECL IOptimizeSprintf(char* buffer, size_t bufferSize, const char* fmt, ...);

IOPTIMIZE_API void* __IOPTIMIZE_STDCALL IOptimizeLoadFunction(wchar_t* libraryName, wchar_t* functionName);

IOPTIMIZE_API void __IOPTIMIZE_STDCALL IOptimizeConvertIOptimizePowerSchemeSubGroupToGUID(IOptimizePowerSchemeSubGroup subGroup, GUID* guid);

#if !defined(NDEBUG)
#define IOPTIMIZE_ASSERT(condition, msg) assert((condition) && msg)
#define IOPTIMIZE_STATIC_ASSERT(condition, msg) static_assert((condition), msg)
#else 
#define IOPTIMIZE_ASSERT(condition, msg)
#define IOPTIMIZE_STATIC_ASSERT(condition, msg)
#endif

#ifdef __cplusplus
}
#endif