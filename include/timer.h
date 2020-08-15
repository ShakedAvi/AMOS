#ifndef TIMER_H
#define TIMER_H

#include "types.h"
#include "isr.h"
#include "system.h"

static void timer_callback(registers_t regs);
void init_timer(uint32 frequency);

#endif
