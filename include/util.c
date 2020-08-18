#include "util.h"

void memory_copy(char *source, char *dest, int nbytes)
{
    int i = 0;
    for (i; i < nbytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}

void memory_set(void *dest, uint32 val, uint32 len)
{
  uint8 value = (uint8)(val & 0xFF);
  uint8 *dest2 = (uint8*)(dest);

  int i = 0;

  while(i < len)
  {
    dest2[i] = value;
    i++;
  }
}

void int_to_ascii(int n, char str[])
{
    int i = 0, sign = 0;
    if ((sign = n) < 0)
    {
      n = -n;
    }

    do
    {
      str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
    {
      str[i++] = '-';
    }
    str[i] = '\0';
    reverse(str, strLen(str));
}
