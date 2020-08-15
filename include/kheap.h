#ifndef ALLOC_H
#define ALLOC_H

#include "types.h"

static uint32 placement_address;

uint32 kmalloc(uint32 sz);
uint32 kmalloc_p(uint32 sz, uint32 *phys);
uint32 kmalloc_ap(uint32 sz, int align, uint32 *phys);

#endif
