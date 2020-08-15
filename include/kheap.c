#include "kheap.h"

uint32 kmalloc(uint32 sz)
{
  uint32 tmp = placement_address;
  placement_address += sz;
  return tmp;
}

uint32 kmalloc_p(uint32 sz, uint32 *phys)
{
  if (phys)
  {
    *phys = placement_address;
  }
  uint32 tmp = placement_address;
  placement_address += sz;
  return tmp;
}

uint32 kmalloc_ap(uint32 sz, int align, uint32 *phys)
{
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys)
  {
    *phys = placement_address;
  }
  uint32 tmp = placement_address;
  placement_address += sz;
  return tmp;
}
