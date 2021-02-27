#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "screen.h"
#include "system.h"
#include "types.h"
#include "isr.h"

#define KEYBOARD_IRQ 33

char readChar();
void readStr(string buffstr);
void init_keyboard();

#endif
