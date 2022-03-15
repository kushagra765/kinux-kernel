#ifndef _LIB_STRING_H
#define _LIB_STRING_H

#include "stddef.h"

size_t strlen(const char *str);
char *strcpy(char *str1, const char *str2);
int strcmp(const char *str1, const char *str2);
int strstr(char *str1, char *str2);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
void *memcpy(void *dest, const void *src, size_t cnt);
void *memset(void *ptr, int ch, size_t n);

#endif //_LIB_STRING_H
