#ifndef STRING_H
#define STRING_H

#include "types.h"

uint16 strLen(string str);
uint16 strEql(string str1, string str2);
string strCpy(string strDest, const string strSrc);
void reverse(char str[], int length);

#endif
