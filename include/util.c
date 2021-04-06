#include "util.h"

/*
  The function compares between two memory strings.

  Input:
    The strings to compare and their length.
  Output:
    If the strings are equal (0 if they are).
*/
int memory_compare(int8* str1, int8* str2, uint32 len)
{
  const uint8* s1 = (const uint8*)str1;
  const uint8* s2 = (const uint8*)str2;
  while (len-- > 0)
  {
    if (*s1++ != *s2++)
    {
      return s1[-1] < s2[-1] ? -1 : 1;
    }
  }
  return 0;
}

/*
  The function copies a memory string to another one.

  Input:
    The source and dest strings and the length to copy.
  Output:
    None.
*/
void memory_copy(int8* src, int8* dest, int n)
{
  int8* csrc = (int8*)src;
  int8* cdest = (int8*)dest;

  for (int i = 0; i < n; i++)
  {
    cdest[i] = csrc[i];
  }
}

/*
  The function sets any byte in a memory string with the same value.

  Input:
    The string, the value to set the string with and the length to set.
  Output:
    None.
*/
void memory_set(void* dest, uint32 val, uint32 len)
{
  uint8 value = (uint8)(val & 0xFF);
  uint8* dest2 = (uint8*)(dest);

  int i = 0;

  while(i < len)
  {
    dest2[i] = value;
    i++;
  }
}

/*
  The function turns an integer into a string.

  Input:
    The integer to convert, the buffer to convert the integer into
    and the conversion base (decimal, hex etc).
  Output:
    The converted string.
*/
int8* itoa(int value, int8* str, int base)
{
    int8* rc;
    int8* ptr;
    int8* low;
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
        int8 tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}
