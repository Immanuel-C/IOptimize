#include <tests/CommonTest.h>
#include <tests/power/Power.h>
#include <tests/timerResolution/TimerResolution.h>

/**
* Creating new tests:
*	- All tests will be use __IOPTIMIZE_STDCALL calling convention.
*	- All tests must return void.
*	- All tests must have no parameters.
*	- All tests must be executed by the ExecuteTest function (e.g. ExecuteTest( ExampleTest, "Example Test")).
*/

int main() {
	ExecuteTest(TestPower, "IOptimize Test Power Features");
	ExecuteTest(TestTimerResolution, "IOptimize Test Timer Resolution");
	return 0;
}