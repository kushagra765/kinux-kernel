#ifndef _FS_TARFS_H
#define _FS_TARFS_H

#include <stdint.h>

struct tarfs_hdr {
  char filename[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char typeflag[1];
  unsigned int caddr;
};

unsigned int tarfs_getsize(const char *in);
unsigned int tarfs_parse(unsigned int addr);
void tarfs_list_headers();
int init_tarfs(uint32_t loc);

#endif //_FS_TARFS_H
