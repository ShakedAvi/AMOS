#include "idt.h"
#include "idt.h"
#include "screen.h"
#include "util.h"

/*
  The function sets an IDT gate.

  Input:
    The number of the gate and the address of the interrupt handler.
  Output:
    None.
*/
void set_idt_gate(int n, uint32 handler)
{
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E | 0x60;
    idt[n].high_offset = high_16(handler);
}

/*
  The function sets the IDT.

  Input:
    None.
  Output:
    None.
*/
void set_idt()
{
    idt_reg.base = (uint32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
