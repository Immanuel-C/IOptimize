#include "Utils.h"

#include <stdarg.h>
#include <stdio.h>

void IOptimizeLog(const char* fmt, ...) {
	va_list vaArgs;

	va_start(vaArgs, fmt);

	vfprintf(stdout, fmt, vaArgs);

	va_end(vaArgs);
}

void IOptimizeLogErr(const char* fmt, ...) {
	va_list vaArgs;

	va_start(vaArgs, fmt);

	vfprintf(stderr, fmt, vaArgs);

	va_end(vaArgs);

}
