#ifndef STRING_H
#define STRING_H

#include "types.h"

/*
  The function returns the length of a string.

  Input:
    The string to get it's length.
  Output:
    The length of the given string.
*/
uint16 strLen(char* str);

/*
  The function checks if two strings are equal.

  Input:
    The strings to check.
  Output:
    If they are equal (0 if they are not).
*/
uint16 strEql(char* str1, char* str2);

/*
  The function copies a string into another one.

  Input:
    The source and dest strings.
  Output:
    None.
*/
void strCpy(char* strDest, char* strSrc);

/*
  The function turns a string into integer.

  Input:
    The string to turn into integer.
  Output:
    The final integer.
*/
int ascii_to_int(char* str);

/*
  The function reverses a string.

  Input:
    The string to reverse and it's length.
  Output:
    None.
*/
void reverse(char* str, int length);

#endif
