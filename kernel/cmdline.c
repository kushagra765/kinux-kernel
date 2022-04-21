#include <kinux/cmdline.h>
#include <kinux/printm.h>
#include <lib/string.h>

int cmdline_opts[CMDLINE_OPTS] = {0 /* Disable Keyboard */,
                                  0 /* Disable tarfs */};

int cmdline_parse(const char *cmdline) {
  printm("Kernel command line: %s\n", cmdline);

  if (!strcmp(cmdline, "dis_kb")) {
    cmdline_opts[0] = 1;
  } else if (!strcmp(cmdline, "dis_tarfs")) {
    cmdline_opts[1] = 1;
  }

  return 0;
}
