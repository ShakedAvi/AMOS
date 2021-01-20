#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "util.h"
#include "paging.h"

struct gdt_entry_struct
{
   uint16 limit_low;           // The lower 16 bits of the limit.
   uint16 base_low;            // The lower 16 bits of the base.
   uint8  base_middle;         // The next 8 bits of the base.
   uint8  access;              // Access flags, determine what ring this segment can be used in.
   uint8  granularity;
   uint8  base_high;           // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
   uint16 limit;               // The upper 16 bits of all selector limits.
   uint32 base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

struct tss_entry_struct
{
   uint32 prev_tss;
   uint32 esp0;
   uint32 ss0;
   uint32 esp1;
   uint32 ss1;
   uint32 esp2;
   uint32 ss2;
   uint32 cr3;
   uint32 eip;
   uint32 eflags;
   uint32 eax;
   uint32 ecx;
   uint32 edx;
   uint32 ebx;
   uint32 esp;
   uint32 ebp;
   uint32 esi;
   uint32 edi;
   uint32 es;
   uint32 cs;
   uint32 ss;
   uint32 ds;
   uint32 fs;
   uint32 gs;
   uint32 ldt;
   uint16 trap;
   uint16 iomap_base;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

void init_gdt();
static void gdt_set_gate(int32 num, uint32 base, uint32 limit, uint8 access, uint8 gran);
void set_kernel_stack(uint32 stack);

#endif
