#ifndef STRING_H
#define STRING_H

#include "types.h"

uint16 strLen(string str);
uint16 strEql(string str1, string str2);
void strCpy(string strDest, const string strSrc);
string strTok(string str, string comp);
int ascii_to_int(string str);
void reverse(char str[], int length);

#endif
