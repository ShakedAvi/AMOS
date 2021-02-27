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

void strCpy(string strDest, string strSrc)
{
  int i = 0;
  do
  {
    strDest[i] = strSrc[i];
  }
  while(strSrc[++i] != 0);
}

string strTok(string str, string comp)
{
	static int pos;
	static char *s;
	int i =0, start = pos;

	if(str != 0)
		s = str;

	i = 0;
	int j = 0;
	while(s[pos] != '\0')
	{
		j = 0;
		while(comp[j] != '\0')
		{
			if(s[pos] == comp[j])
			{
				s[pos] = '\0';
				pos = pos+1;
				if(s[start] != '\0')
					return (&s[start]);
				else
				{
					start = pos;
					pos--;
					break;
				}
			}
			j++;
		}
		pos++;
	}
	s[pos] = '\0';
	if(s[start] == '\0')
		return 0;
	else
		return &s[start];
}

int ascii_to_int(string str)
{
    int res = 0;

    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
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
