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

uint16 inports(uint16 port)
{
  uint16 rv;
  __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

void outports(uint16 port, uint16 data)
{
  __asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data)); //Might not work cuz assembly is fun
}

uint16 inportl(uint16 port)
{
  uint32 rv;
  __asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

void outportl(uint16 port, uint32 data)
{
  __asm__ __volatile__ ("outl %1, %0" : : "dN" (port), "a" (data));
}

uint8 read_register(uint8 reg)
{
    outportb(0x70, reg);
    return inportb(0x71);
}

void write_register(uint8 reg, uint8 value)
{
    outportb(0x70, reg);
    outportb(0x71, value);
}

uint8 bcd2bin(uint8 bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
