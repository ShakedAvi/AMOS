#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "util.h"

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
   uint32 prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
   uint32 esp0;       // The stack pointer to load when we change to kernel mode.
   uint32 ss0;        // The stack segment to load when we change to kernel mode.
   uint32 esp1;       // Unused...
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
   uint32 es;         // The value to load into ES when we change to kernel mode.
   uint32 cs;         // The value to load into CS when we change to kernel mode.
   uint32 ss;         // The value to load into SS when we change to kernel mode.
   uint32 ds;         // The value to load into DS when we change to kernel mode.
   uint32 fs;         // The value to load into FS when we change to kernel mode.
   uint32 gs;         // The value to load into GS when we change to kernel mode.
   uint32 ldt;        // Unused...
   uint16 trap;
   uint16 iomap_base;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

void init_gdt();
static void gdt_set_gate(int32 num, uint32 base, uint32 limit, uint8 access, uint8 gran);

#endif
