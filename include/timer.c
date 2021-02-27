#include "timer.h"

extern void timer_handler();

void init_timer(uint32 frequency)
{
   register_interrupt_handler(TIMER_IRQ, (interrupt_handler_t)&timer_handler);

   uint32 divisor = 1193182 / frequency;

   outportb(0x43, 0x34);

   uint8 l = (uint8)(divisor & 0xFF);
   uint8 h = (uint8)((divisor>>8) & 0xFF);

   outportb(0x40, l);
   outportb(0x40, h);
}
