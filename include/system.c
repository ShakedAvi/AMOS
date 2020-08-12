#include "system.h"

uint8 inportb(uint16 port)
{
  uint8 rv;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

void outportb(uint16 port, uint8 data)
{
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}
