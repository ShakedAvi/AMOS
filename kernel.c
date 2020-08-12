#include "include/types.h"
#include "include/string.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/isr.h"

int kmain()
{
  isr_install();
  clearScreen();

  print("     _    __  __  ___  ____  \n");
  print("    / \\  |  \\/  |/ _ \\/ ___| \n");
  print("   / _ \\ | |\\/| | | | \\___ \\ \n");
  print("  / ___ \\| |  | | |_| |___) |\n");
  print(" /_/   \\_\\_|  |_|\\___/|____/ \n");

  int exitFlag = 1;

  while(exitFlag)
  {
    print("\n>> ");
    readStr();
  }

  return 0;
}
