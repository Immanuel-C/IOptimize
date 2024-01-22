#pragma once

#include <stdint.h>
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IOPTIMIZE_EXPORT_DLL
	#define IOPTIMIZE_API __declspec(dllexport)
#else 
	#define IOPTIMIZE_API __declspec(dllimport)
#endif


#ifndef TRUE
#define TRUE 1
#endif 

#ifndef FALSE
#define FALSE 1
#endif 

#ifndef NULLPTR
#define NULLPTR (void*)0
#endif


struct IOptimizeVector2 {
	union { float width, x;  };
	union { float height, y; };
};


typedef uint64_t Flags;
typedef Flags IOptimizeTypeFlags;

enum IOptimizeTypeFlagBits {
	IOPTIMIZE_TYPE_NONE = 0x0000000,
	IOPTIMIZE_TYPE_REVERT = 0x00000001,
	IOPTIMIZE_TYPE_FPS = 0x00000002,
	IOPTIMIZE_TYPE_LATENCY = 0x00000004,
	// Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
	IOPTIMIZE_TYPE_FORTNITE = 0x00000008,
	// Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
	IOPTIMIZE_TYPE_APEX_LEGENDS = 0x00000010,
};

struct IOptimizeTimerResolutionValues {
	uint32_t maxResolution, minResolution, currResolution;
};

#ifdef __cplusplus
}
#endif