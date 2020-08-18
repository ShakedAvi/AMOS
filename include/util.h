#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "string.h"

void memory_copy(char *source, char *dest, int nbytes);
void memory_set(void *dest, uint32 val, uint32 len);
void int_to_ascii(int n, char str[]);

#endif
