#include "isr.h"

/* To print the message which defines every exception */
string exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void register_interrupt_handler(uint8 n, interrupt_handler_t handler)
{
    interrupt_handlers[n] = handler;
    set_idt_gate(n, (uint32)handler);
}

void isr_install()
{
  memory_set(&interrupt_handlers, 0, sizeof (interrupt_handler_t) * IDT_ENTRIES);

  idt_reg.limit = sizeof (idt_gate_t) * IDT_ENTRIES - 1;
  idt_reg.base  = (uint32) &idt;

  memory_set(&idt, 0, sizeof (idt_gate_t) * IDT_ENTRIES - 1);

  outportb(0x20, 0x11);
  outportb(0xA0, 0x11);
  outportb(0x21, 0x20);
  outportb(0xA1, 0x28);
  outportb(0x21, 0x04);
  outportb(0xA1, 0x02);
  outportb(0x21, 0x01);
  outportb(0xA1, 0x01);
  outportb(0x21, 0x0);
  outportb(0xA1, 0x0);

  set_idt_gate(0, (uint32)isr0);
  set_idt_gate(1, (uint32)isr1);
  set_idt_gate(2, (uint32)isr2);
  set_idt_gate(3, (uint32)isr3);
  set_idt_gate(4, (uint32)isr4);
  set_idt_gate(5, (uint32)isr5);
  set_idt_gate(6, (uint32)isr6);
  set_idt_gate(7, (uint32)isr7);
  set_idt_gate(8, (uint32)isr8);
  set_idt_gate(9, (uint32)isr9);
  set_idt_gate(10, (uint32)isr10);
  set_idt_gate(11, (uint32)isr11);
  set_idt_gate(12, (uint32)isr12);
  set_idt_gate(13, (uint32)isr13);
  set_idt_gate(14, (uint32)isr14);
  set_idt_gate(15, (uint32)isr15);
  set_idt_gate(16, (uint32)isr16);
  set_idt_gate(17, (uint32)isr17);
  set_idt_gate(18, (uint32)isr18);
  set_idt_gate(19, (uint32)isr19);
  set_idt_gate(20, (uint32)isr20);
  set_idt_gate(21, (uint32)isr21);
  set_idt_gate(22, (uint32)isr22);
  set_idt_gate(23, (uint32)isr23);
  set_idt_gate(24, (uint32)isr24);
  set_idt_gate(25, (uint32)isr25);
  set_idt_gate(26, (uint32)isr26);
  set_idt_gate(27, (uint32)isr27);
  set_idt_gate(28, (uint32)isr28);
  set_idt_gate(29, (uint32)isr29);
  set_idt_gate(30, (uint32)isr30);
  set_idt_gate(31, (uint32)isr31);

  set_idt_gate (32, (uint32)irq0);
  set_idt_gate (33, (uint32)irq1);
  set_idt_gate (34, (uint32)irq2);
  set_idt_gate (35, (uint32)irq3);
  set_idt_gate (36, (uint32)irq4);
  set_idt_gate (37, (uint32)irq5);
  set_idt_gate (38, (uint32)irq6);
  set_idt_gate (39, (uint32)irq7);
  set_idt_gate (40, (uint32)irq8);
  set_idt_gate (41, (uint32)irq9);
  set_idt_gate (42, (uint32)irq10);
  set_idt_gate (43, (uint32)irq11);
  set_idt_gate (44, (uint32)irq12);
  set_idt_gate (45, (uint32)irq13);
  set_idt_gate (46, (uint32)irq14);
  set_idt_gate (47, (uint32)irq15);

  set_idt(); // Load with ASM

}

/*Handlers*/
void isr0()
{
    print(exception_messages[0]);
    asm("hlt");
}
void isr1()
{
    print(exception_messages[1]);
    asm("hlt");
}
void isr2()
{
    print(exception_messages[2]);
    asm("hlt");
}
void isr3()
{
    print(exception_messages[3]);
    asm("hlt");
}
void isr4()
{
    print(exception_messages[4]);
    asm("hlt");
}
void isr5()
{
    print(exception_messages[5]);
    asm("hlt");
}
void isr6()
{
    print(exception_messages[6]);
    asm("hlt");
}
void isr7()
{
    print(exception_messages[7]);
    asm("hlt");
}
void isr8()
{
    print(exception_messages[8]);
    asm("hlt");
}
void isr9()
{
    print(exception_messages[9]);
    asm("hlt");
}
void isr10()
{
    print(exception_messages[10]);
    asm("hlt");
}
void isr11()
{
    print(exception_messages[11]);
    asm("hlt");
}
void isr12()
{
    print(exception_messages[12]);
    asm("hlt");
}
void isr13()
{
    print(exception_messages[13]);
    asm("hlt");
}
void isr14()
{
    print(exception_messages[14]);
    asm("hlt");
}
void isr15()
{
    print(exception_messages[15]);
    asm("hlt");
}
void isr16()
{
    print(exception_messages[16]);
    asm("hlt");
}
void isr17()
{
    print(exception_messages[17]);
    asm("hlt");
}
void isr18()
{
    print(exception_messages[18]);
    asm("hlt");
}
void isr19()
{
    print(exception_messages[19]);
    asm("hlt");
}
void isr20()
{
    print(exception_messages[20]);
    asm("hlt");
}
void isr21()
{
    print(exception_messages[21]);
    asm("hlt");
}
void isr22()
{
    print(exception_messages[22]);
    asm("hlt");
}
void isr23()
{
    print(exception_messages[23]);
    asm("hlt");
}
void isr24()
{
    print(exception_messages[24]);
    asm("hlt");
}
void isr25()
{
    print(exception_messages[25]);
    asm("hlt");
}
void isr26()
{
    print(exception_messages[26]);
    asm("hlt");
}
void isr27()
{
    print(exception_messages[27]);
    asm("hlt");
}
void isr28()
{
    print(exception_messages[28]);
    asm("hlt");
}
void isr29()
{
    print(exception_messages[29]);
    asm("hlt");
}
void isr30()
{
    print(exception_messages[30]);
    asm("hlt");
}
void isr31()
{
    print(exception_messages[31]);
    asm("hlt");
}

void irq_handler(registers_t *regs)
{
    if (regs->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }
    outportb(0x20, 0x20);

    if (interrupt_handlers[regs->int_no] != 0)
    {
      interrupt_handlers[regs->int_no] (regs);
    }
}

/*End Handlers*/
