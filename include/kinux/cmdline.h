#ifndef _KINUX_CMDLINE_H
#define _KINUX_CMDLINE_H

#define CMDLINE_OPTS 2

int cmdline_parse(const char *cmdline);

extern int cmdline_opts[CMDLINE_OPTS];

#endif //_KINUX_CMDLINE_H
