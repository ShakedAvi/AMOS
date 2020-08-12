#include "string.h"

uint16 stringlen(string str)
{
  uint16 i = 1;
  while(str[i++]);
  return --i;
}

uint16 strEql(string str1, string str2)
{
  uint8 result = 1, i = 0, size = stringlen(str1);

  if(size != stringlen(str2))
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
