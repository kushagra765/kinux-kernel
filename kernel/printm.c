#include <kinux/console.h>
#include <lib/stdarg.h>
#include <lib/stdio.h>

static char buf[1024];

void printm(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  console_write("[ kernel ] ");
  console_write(buf);
  va_end(args);
}
