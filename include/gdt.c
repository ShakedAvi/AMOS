#include "gdt.h"

extern void gdt_flush(uint32);
extern void tss_flush();

static void gdt_set_gate(int32,uint32,uint32,uint8,uint8);
static void write_tss(int32,uint16,uint32);

gdt_entry_t gdt_entries[6];
gdt_ptr_t   gdt_ptr;

tss_entry_t tss_entry;

void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base  = (uint32)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
   write_tss(5, 0x10, 0x0);

   gdt_flush((uint32)&gdt_ptr);
}

static void gdt_set_gate(int32 num, uint32 base, uint32 limit, uint8 access, uint8 gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}

static void write_tss(int32 num, uint16 ss0, uint32 esp0)
{
   uint32 base = (uint32) &tss_entry;
   uint32 limit = base + sizeof(tss_entry);

   gdt_set_gate(num, base, limit, 0xE9, 0x00);

   memory_set(&tss_entry, 0, sizeof(tss_entry));

   tss_entry.ss0  = ss0;  // Set the kernel stack segment.
   tss_entry.esp0 = esp0; // Set the kernel stack pointer.

   tss_entry.cs = 0x0b;
   tss_entry.ss = 0x13;
   tss_entry.ds = 0x13;
   tss_entry.es = 0x13;
   tss_entry.fs = 0x13;
   tss_entry.gs = 0x13;
}

void set_kernel_stack(uint32 stack)
{
   tss_entry.esp0 = stack;
}
