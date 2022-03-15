#include <lib/string.h>
#include <stdint.h>

size_t strlen(const char *str) {
  size_t l;
  while (str[l]) {
    l++;
  }
  return l;
}

char *strcpy(char *str1, const char *str2) {
  char *rc = str1;

  while ((*str1++ = *str2++)) {
  }

  return rc;
}

int strcmp(const char *str1, const char *str2) {
  while ((*str1) && (*str1 == *str2)) {
    ++str1;
    ++str2;
  }

  return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

int strstr(char *str1, char *str2) {
  int i = 0, j = 0;
  int nTemp = i;
  int str1len = strlen(str1);
  int str2len = strlen(str2);
  for (i = 0; i < str1len - str2len; i++) {
    nTemp = i;
    for (j = 0; j < str2len; j++) {
      if (str1[nTemp] == str2[j]) {
        if (j == str2len - 1) {
          return 1;
        }
        nTemp++;
      } else {
        break;
      }
    }
  }
  return 0;
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
