#ifndef ERROR_ERROR_H
#define ERROR_ERROR_H

#include "util/view.h"

#define ERROR_NLINE -1

void Error_setProgramCode(View code);

void Error_assert(bool condition, int line, const char *msg, ...);
void Error_assertnl(bool condition, const char *msg, ...);
[[noreturn]] void Error_raise(int line, const char *msg, ...);
[[noreturn]] void Error_exit(int code);

#endif
