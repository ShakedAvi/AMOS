#include "string.h"

uint16 strLen(string str)
{
  uint16 i = 1;
  while(str[i++]);
  return --i;
}

uint16 strEql(string str1, string str2)
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

string strCpy(string strDest, string strSrc)
{
    char *temp = strDest;
    while(*strDest++ = *strSrc++);
    return temp;
}

void reverse(char str[], int length)
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
