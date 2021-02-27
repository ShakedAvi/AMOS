#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"

int myRand();

uint8 inportb(uint16 port);
void outportb(uint16 port, uint8 data);
uint16 inports(uint16 port);
void outports(uint16 port, uint16 data);
uint16 inportl(uint16 port);
void outportl(uint16 port, uint32 data);

uint8 read_register(uint8 reg);
void write_register(uint8 reg, uint8 value);
uint8 bcd2bin(uint8 bcd);

#endif
