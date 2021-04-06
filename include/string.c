#include "string.h"

/*
  The function returns the length of a string.

  Input:
    The string to get it's length.
  Output:
    The length of the given string.
*/
uint16 strLen(char* str)
{
  uint16 i = 1;
  while(str[i++]);
  return --i;
}

/*
  The function checks if two strings are equal.

  Input:
    The strings to check.
  Output:
    If they are equal (0 if they are not).
*/
uint16 strEql(char* str1, char* str2)
{
  uint8 result = 1, i = 0, size = strLen(str1);

  if(size != strLen(str2))
  {
    result = 0;
  }
  else
  {
    for(i; i <= size; i++)
    {
      if(str1[i] != str2[i])
      {
        result = 0;
      }
    }
  }
  return result;
}

/*
  The function copies a string into another one.

  Input:
    The source and dest strings.
  Output:
    None.
*/
void strCpy(char* strDest, char* strSrc)
{
  int i = 0;
  do
  {
    strDest[i] = strSrc[i];
  }
  while(strSrc[++i] != 0);
}

/*
  The function turns a string into integer.

  Input:
    The string to turn into integer.
  Output:
    The final integer.
*/
int ascii_to_int(char* str)
{
    int res = 0;

    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
}

/*
  The function reverses a string.

  Input:
    The string to reverse and it's length.
  Output:
    None.
*/
void reverse(char* str, int length)
{
    int start = 0;
    int end = length -1;
    char temp = 0;

    while (start < end)
    {
        temp = *(str+end);
        *(str+end) = *(str+start);
        *(str+start) = temp;

        start++;
        end--;
    }
}
