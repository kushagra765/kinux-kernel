#include "tarfs.h"
#include <kinux/printm.h>
#include <lib/string.h>
#include <stdint.h>

struct tarfs_hdr *headers[32];
int nr_files = 0;
int found_init = 0;
struct tarfs_hdr *init_hdr;

unsigned int tarfs_getsize(const char *in) {
  unsigned int size = 0;
  unsigned int count = 1;
  unsigned int j;

  for (j = 11; j > 0; j--, count *= 8) {
    size += ((in[j - 1] - '0') * count);
  }

  return size;
}

unsigned int tarfs_parse(unsigned int addr) {
  unsigned int i;

  for (i = 0;; i++) {
    struct tarfs_hdr *header = (struct tarfs_hdr *)addr;

    if (header->filename[0] == '\0') {
      break;
    }

    unsigned int size = tarfs_getsize(header->size);
    headers[i] = header;
    addr += ((size / 512) + 1) * 512;
    header->caddr = addr;

    if (size % 512) {
      addr += 512;
    }
  }

  nr_files = i;
  return i;
}

void tarfs_list_headers() {
  printm("tarfs: listing headers...\n");
  if (nr_files == 0) {
    printm("tarfs: no files\n");
    return;
  }
  char cbuf[256];
  for (int i = 0; i < nr_files; i++) {
    strcpy(cbuf, headers[i]->caddr);
    printm("  %s content: %s\n", headers[i]->filename, cbuf);
  }
}

int init_tarfs(uint32_t loc) {
  tarfs_parse(loc);
  printm("tarfs: parsed initrd. loc=0x%x\n", loc);
}
