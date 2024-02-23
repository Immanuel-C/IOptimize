#include <tests/CommonTest.h>
#include <tests/power/Power.h>
#include <tests/timerResolution/TimerResolution.h>

int main() {
	ExecuteTest(TestPower, "IOptimize Test Power Features");
	ExecuteTest(TestTimerResolution, "IOptimize Test Timer Resolution");
	return 0;
}