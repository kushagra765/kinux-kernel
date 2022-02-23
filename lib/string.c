#include <lib/string.h>
#include <stdint.h>

size_t strlen(const char *str) {
  size_t l;
  while (str[l]) {
    l++;
  }
  return l;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val,
                        size_t count) {
  unsigned short *tmp = (unsigned short *)dest;
  for (; count != 0; count--) {
    *tmp++ = val;
  }
  return dest;
}

void *memcpy(void *dest, const void *src, size_t cnt) {
  const char *src_ptr = (const char *)src;
  char *dest_ptr = (char *)dest;
  for (; cnt != 0; cnt--) {
    *dest_ptr++ = *src_ptr++;
  }
  return dest;
}

void *memset(void *ptr, int ch, size_t n) {
  uint8_t *p = (uint8_t *)ptr;
  while (n--) {
    *p++ = (uint8_t)ch;
  }
  return ptr;
}
