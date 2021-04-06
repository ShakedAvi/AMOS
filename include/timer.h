#ifndef TIMER_H
#define TIMER_H

#include "types.h"
#include "isr.h"

#define TIMER_IRQ 32

/*
  The function initializes the timer.

  Input:
    The tick's frequency.
  Output:
    None.
*/
void init_timer(uint32 frequency);

#endif
