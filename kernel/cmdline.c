#include <kinux/cmdline.h>
#include <kinux/printm.h>
#include <lib/string.h>

int cmdline_opts[CMDLINE_OPTS] = {0 /* Disable Keyboard */,
                                  0 /* Disable tarfs */};

int cmdline_parse(const char *cmdline) {
  printm("Kernel command line: %s\n", cmdline);

  if (strstr(cmdline, "dis_kb") == 0) {
    cmdline_opts[0] = 1;
  } else if (strstr(cmdline, "dis_tarfs") == 0) {
    cmdline_opts[1] = 1;
  }

  return 0;
}
