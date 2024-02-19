#pragma once

#include <stdint.h>
#include <Windows.h>
#include <assert.h>

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


typedef struct IOptimizeVector2_T {
	union { float width, x;  };
	union { float height, y; };
} IOptimizeVector2;


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

typedef struct IOptimizeTimerResolutionValues_T {
	uint32_t maxResolution, minResolution, currResolution;
} IOptimizeTimerResolutionValues;

typedef enum IOptimizeBoolValues {
	IOPTIMIZE_FALSE = 0x0,
	IOPTIMIZE_TRUE = 0x1,
} IOptimizeBool;

IOPTIMIZE_API void IOptimizeLog(const char* fmt, ...);
IOPTIMIZE_API void IOptimizeLogErr(const char* fmt, ...);

#if !defined(NDEBUG)
#define IOPTIMIZE_ASSERT(condition, msg) assert((condition) && msg)
#else 
#define IOPTIMIZE_ASSERT(condition, msg)
#endif

#ifdef __cplusplus
}
#endif