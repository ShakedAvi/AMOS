#ifndef MOUSE_H
#define MOUSE_H

#include "isr.h"
#include "graphic.h"

#define MOUSE_IRQ 44

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64

void mouse_handler();

void mouse_wait(uint8 a_type);
void mouse_write(uint8 a_write);
uint8 mouse_read();

void init_mouse();

#endif
