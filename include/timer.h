#ifndef TIMER_H
#define TIMER_H

#include "types.h"
#include "isr.h"

#define TIMER_IRQ 32

void init_timer(uint32 frequency);

#endif
