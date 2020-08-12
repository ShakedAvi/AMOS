#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"
#include "system.h"
#include "string.h"

static int cursorX = 0, cursorY = 0;

static const uint8 sw = 80, sh = 25, sd = 2;  //We define the screen width, height, and depth.

void clearLine(uint8 from,uint8 to);
void updateCursor();
void clearScreen();
void scrollUp(uint8 lineNumber);
void newLineCheck();
void printch(char c);
void print(string str);

#endif
