#include "system.h"

/*
  The function reads a byte from a port.

  Input:
    The port to read from.
  Output:
    The read byte.
*/
uint8 inportb(uint16 port)
{
  uint8 rv;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

/*
  The function writes a byte to a port.

  Input:
    The port to write to and the byte to write.
  Output:
    None.
*/
void outportb(uint16 port, uint8 data)
{
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

/*
  The function reads a word from a port.

  Input:
    The port to read from.
  Output:
    The read word.
*/
uint16 inports(uint16 port)
{
  uint16 rv;
  __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

/*
  The function writes a word to a port.

  Input:
    The port to write to and the word to write.
  Output:
    None.
*/
void outports(uint16 port, uint16 data)
{
  __asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data));
}

/*
  The function reads a double word from a port.

  Input:
    The port to read from.
  Output:
    The read double word.
*/
uint32 inportl(uint16 port)
{
  uint32 rv;
  __asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

/*
  The function writes a double word to a port.

  Input:
    The port to write to and the double word to write.
  Output:
    None.
*/
void outportl(uint16 port, uint32 data)
{
  __asm__ __volatile__ ("outl %1, %0" : : "dN" (port), "a" (data));
}

/*
  The function reads byte from a register.

  Input:
    The register to read from.
  Output:
    The read byte from the register.
*/
uint8 read_register(uint8 reg)
{
    outportb(0x70, reg);
    return inportb(0x71);
}

/*
  The function writes a byte to a register.

  Input:
    The register to write to and the byte to write.
  Output:
    None.
*/
void write_register(uint8 reg, uint8 value)
{
    outportb(0x70, reg);
    outportb(0x71, value);
}

/*
  The function converts a BCD (Binary Coded Decimal) number into a normal binary number.

  Input:
    The BCD to convert.
  Output:
    The final binary number.
*/
uint8 bcd2bin(uint8 bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
