#include "system.h"

static unsigned long int next = 1;

int myRand()
{
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

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
