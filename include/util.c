#include "util.h"

int memory_compare(char* str1, char* str2, uint32 len)
{
  const unsigned char *s1 = (const unsigned char*)str1;
  const unsigned char *s2 = (const unsigned char*)str2;
  while (len-- > 0)
  {
    if (*s1++ != *s2++)
    {
      return s1[-1] < s2[-1] ? -1 : 1;
    }
  }
  return 0;
}

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

char *itoa(int value, char *str, int base)
{
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if (value < 0 && base == 10)
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

int otob(unsigned char *str, int size)
{
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}
