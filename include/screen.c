#include "screen.h"

/*
  The function clears a line of the screen.

  Input:
    The start and the end of the line.
  Output:
    None.
*/
void clearLine(uint8 from, uint8 to)
{
  uint16 i = sw * from * sd;
  char* vidmem = (char*)0xb8000;
  for(i; i < (sw * to * sd); i++)
  {
    vidmem[i] = 0x0;
  }
}

/*
  The function updates the place of the cursor.

  Input:
    None.
  Output:
    None.
*/
void updateCursor()
{
  unsigned temp;

  temp = cursorY * sw + cursorX;

  outportb(0x3D4, 14);
  outportb(0x3D5, temp >> 8);
  outportb(0x3D4, 15);
  outportb(0x3D5, temp);
}

/*
  The function clears the screen.

  Input:
    None.
  Output:
    None.
*/
void clearScreen()
{
  clearLine(0,sh-1);
  cursorX = 0;
  cursorY = 0;
  updateCursor();
}

/*
  The function scrolls the screen up by one line.

  Input:
    The number of the line to scroll up from.
  Output:
    None.
*/
void scrollUp(uint8 lineNumber)
{
  char* vidmem = (char*)0xb8000;
  uint16 i = 0;

  clearLine(0, lineNumber-1);

  for (i;i<sw*(sh-1)*2;i++)
  {
    vidmem[i] = vidmem[i+sw*2*lineNumber];
  }

  clearLine(sh-1-lineNumber,sh-1);

  if((cursorY - lineNumber) < 0 )
  {
    cursorY = 0;
    cursorX = 0;
  }
  else
  {
    cursorY -= lineNumber;
  }
  updateCursor();
}

/*
  The function checks if we need to scroll up the screen.

  Input:
    None.
  Output:
    None.
*/
void newLineCheck()
{
  if(cursorY >= sh-1)
  {
    scrollUp(1);
  }
}

/*
  The function prints a character to the screen.

  Input:
    The character to print.
  Output:
    None.
*/
void printch(char c)
{
  char* vidmem = (char*) 0xb8000;
  switch(c)
  {
    case ('\b'):
      if(cursorX > 0)
      {
	       cursorX--;
         vidmem[(cursorY * sw + cursorX)*sd]=0x00;
	    }
	    break;
    case ('\r'):
      cursorX = 0;
      break;
    case ('\n'):
      cursorX = 0;
      cursorY++;
      break;
    default:
      vidmem [((cursorY * sw + cursorX))*sd] = c;
      vidmem [((cursorY * sw + cursorX))*sd+1] = 0x0F;
      cursorX++;
      break;
  }

  if(cursorX >= sw)
  {
    cursorX = 0;
    cursorY++;
  }

  updateCursor();
  newLineCheck();
}

/*
  The function prints a string to the screen.

  Input:
    The string to print.
  Output:
    None.
*/
void print(char* str)
{
  uint16 i = 0;
  uint8 length = strLen(str);
  for(i; i < length; i++)
  {
    printch(str[i]);
  }
}
