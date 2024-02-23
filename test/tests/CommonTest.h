#pragma once

#include <IOptimize.h>
#include "tests/Utils.h"

typedef void (__IOPTIMIZE_STDCALL DefualtTest)();

void __IOPTIMIZE_VECTORCALL ExecuteTest(DefualtTest test, const char* testName);