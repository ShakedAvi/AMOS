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

  char currCommand[200] = { 0 };

  while(exitFlag)
  {
    print("\n>> ");
    readStr(currCommand);
    if (strEql(currCommand, "exit"))
    {
      print("\nGoodbye!\n");
      exitFlag = 0;
    }
  }

  return 0;
}
