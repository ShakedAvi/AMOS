#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "util.h"

#define KERNEL_CS 0x08

typedef struct idt_gate_t
{
    uint16 low_offset;
    uint16 sel;
    uint8 always0;
    uint8 flags;
    uint16 high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct idt_register_t
{
    uint16 limit;
    uint32 base;
} __attribute__((packed)) idt_register_t;

typedef struct registers32
{
   uint32 ds;                  // Data segment selector
   uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32 int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers32_t;

typedef struct registers16
{
   uint16 di, si, bp, sp, bx, dx, cx, ax;
   uint16 gs, fs, es, ds, eflags;
} registers16_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

typedef void (*interrupt_handler_t)(registers32_t *);
interrupt_handler_t interrupt_handlers[IDT_ENTRIES];

/*
  The function sets an IDT gate.

  Input:
    The number of the gate and the address of the interrupt handler.
  Output:
    None.
*/
void set_idt_gate(int n, uint32 handler);

/*
  The function sets the IDT.

  Input:
    None.
  Output:
    None.
*/
void set_idt();

#endif
