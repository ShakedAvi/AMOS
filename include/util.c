#include "util.h"

void memory_copy(char *source, char *dest, int nbytes)
{
    int i = 0;
    for (i; i < nbytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}

void memory_set(uint8 *dest, uint8 val, uint32 len)
{
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--)
    {
      *temp++ = val;
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
}
