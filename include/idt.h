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

typedef struct registers
{
   uint32 ds;                  // Data segment selector
   uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32 int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

typedef void (*interrupt_handler_t)(registers_t *);
interrupt_handler_t interrupt_handlers[IDT_ENTRIES];

void set_idt_gate(int n, uint32 handler);
void set_idt();

#endif
