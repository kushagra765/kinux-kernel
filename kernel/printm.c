#include <kinux/console.h>
#include <kinux/printm.h>
#include <lib/stdarg.h>
#include <lib/stdio.h>

static char buf[1024];
int printm_dis_hdr = 0;

void printm(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  if (!printm_dis_hdr) {
    console_write("[ kernel ] ");
  }
  console_write(buf);
  va_end(args);
}
