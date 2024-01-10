#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
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

typedef uint64_t Flags;
typedef Flags IOptimizeTypeFlags;

enum IOptimizeTypeFlagBits {
	IOPTIMIZE_TYPE_NONE = 0x0000000ULL,
	IOPTIMIZE_TYPE_REVERT = 0x00000001ULL,
	IOPTIMIZE_TYPE_FPS = 0x00000002ULL,
	IOPTIMIZE_TYPE_LATENCY = 0x00000004ULL,
	// Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
	IOPTIMIZE_TYPE_FORTNITE = 0x00000008ULL,
	// Must also add IOPTIMIZE_TYPE_FPS flag bit when using this flag bit
	IOPTIMIZE_TYPE_APEX_LEGENDS = 0x00000010ULL,
};

#ifdef __cplusplus
}
#endif