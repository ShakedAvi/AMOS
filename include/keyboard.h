#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "screen.h"
#include "system.h"
#include "types.h"
#include "isr.h"

char readChar();
void readStr(string buffstr);
void init_keyboard();

#endif
