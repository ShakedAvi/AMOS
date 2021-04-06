#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"
#include "system.h"
#include "string.h"

static int cursorX = 0, cursorY = 0;

static const uint8 sw = 80, sh = 25, sd = 2;  //We define the screen width, height, and depth.

/*
  The function clears a line of the screen.

  Input:
    The start and the end of the line.
  Output:
    None.
*/
void clearLine(uint8 from,uint8 to);

/*
  The function updates the place of the cursor.

  Input:
    None.
  Output:
    None.
*/
void updateCursor();

/*
  The function clears the screen.

  Input:
    None.
  Output:
    None.
*/
void clearScreen();

/*
  The function scrolls the screen up by one line.

  Input:
    The number of the line to scroll up from.
  Output:
    None.
*/
void scrollUp(uint8 lineNumber);

/*
  The function checks if we need to scroll up the screen.

  Input:
    None.
  Output:
    None.
*/
void newLineCheck();

/*
  The function prints a character to the screen.

  Input:
    The character to print.
  Output:
    None.
*/
void printch(char c);

/*
  The function prints a string to the screen.

  Input:
    The string to print.
  Output:
    None.
*/
void print(char* str);

#endif
