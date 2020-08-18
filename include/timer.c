#include "timer.h"

uint32 tick = 0;

static void timer_callback(registers_t regs)
{
   tick++;
   char currTick[20] = { 0 };
   int_to_ascii(tick, currTick);
   print("Tick: ");
   print(currTick);
   print("\n");
}

void init_timer(uint32 frequency)
{
   register_interrupt_handler(32, (interrupt_handler_t)&timer_callback);

   uint32 divisor = 1193180 / frequency;

   outportb(0x43, 0x36);

   uint8 l = (uint8)(divisor & 0xFF);
   uint8 h = (uint8)( (divisor>>8) & 0xFF );

   outportb(0x40, l);
   outportb(0x40, h);
}
