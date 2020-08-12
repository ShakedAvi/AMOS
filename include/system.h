#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"

uint8 inportb(uint16 port);
void outportb(uint16 port, uint8 data);

#endif
