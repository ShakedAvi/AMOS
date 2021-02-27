#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "string.h"

int memory_compare(char* str1, char* str2, uint32 len);
void memory_copy(char *src, char *dest, int n);
void memory_set(void *dest, uint32 val, uint32 len);
void int_to_ascii(int n, char str[]);
char *itoa(int value, char *str, int base);
int otob(unsigned char *str, int size);

#endif
