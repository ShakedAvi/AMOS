#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "string.h"

/*
  The function compares between two memory strings.

  Input:
    The strings to compare and their length.
  Output:
    If the strings are equal (0 if they are).
*/
int memory_compare(int8* str1, int8* str2, uint32 len);

/*
  The function copies a memory string to another one.

  Input:
    The source and dest strings and the length to copy.
  Output:
    None.
*/
void memory_copy(int8* src, int8* dest, int n);

/*
  The function sets any byte in a memory string with the same value.

  Input:
    The string, the value to set the string with and the length to set.
  Output:
    None.
*/
void memory_set(void* dest, uint32 val, uint32 len);

/*
  The function turns an integer into a string.

  Input:
    The integer to convert, the buffer to convert the integer into
    and the conversion base (decimal, hex etc).
  Output:
    The converted string.
*/
int8* itoa(int value, int8* str, int base);

#endif
