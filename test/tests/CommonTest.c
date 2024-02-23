#include "CommonTest.h"

void __IOPTIMIZE_VECTORCALL ExecuteTest(DefualtTest test, const char* testName) {
	IOptimizeLog("%s\n%s\n", testName, IOPTIMIZE_TEST_BORDER);
	test();
	IOptimizeLog("%s\n", IOPTIMIZE_TEST_BORDER);
}