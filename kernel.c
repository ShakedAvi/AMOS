#include "include/types.h"
#include "include/string.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/isr.h"
#include "include/timer.h"
#include "include/paging.h"

int kmain()
{
  isr_install();

  clearScreen();

  init_paging();

  print("     _    __  __  ___  ____  \n");
  print("    / \\  |  \\/  |/ _ \\/ ___| \n");
  print("   / _ \\ | |\\/| | | | \\___ \\ \n");
  print("  / ___ \\| |  | | |_| |___) |\n");
  print(" /_/   \\_\\_|  |_|\\___/|____/ \n");

  int exitFlag = 1;

  char currCommand[200] = { 0 };

  // Testing Page Fault:
  //uint32 *ptr = (uint32*)0xA0000000;
  //uint32 do_page_fault = *ptr;

  while(exitFlag)
  {
    print("\n>> ");
    readStr(currCommand);
    if (strEql(currCommand, "exit"))
    {
      __asm__ __volatile__("cli");
      print("\nGoodbye!\n");
      exitFlag = 0;
      asm("hlt");
    }
    if (strEql(currCommand, "timer"))
    {
      print("\n");
      __asm__ __volatile__("sti");
      init_timer(50);
    }
  }

  return 0;
}
