#include "include/types.h"
#include "include/string.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/isr.h"
#include "include/timer.h"
#include "include/paging.h"
#include "include/task.h"
#include "include/gdt.h"

static lock_t myLock;

void task_first()
{
  while (1)
	{
    acquire(&myLock);
		print("First!\n");
    release(&myLock);

    //sleep();
	}
}

void task_second()
{
  while (1)
	{
    acquire(&myLock);
		print("Second!\n");
    release(&myLock);

    //sleep();
	}
}

void task_third()
{
  while (1)
	{
    acquire(&myLock);
		print("Third!\n");
    release(&myLock);

    //sleep();
	}
}

void task_fourth()
{
  while (1)
	{
    acquire(&myLock);
		print("Fourth!\n");
    release(&myLock);

    //sleep();
	}
}

void print_menu()
{
  print("     _    __  __  ___  ____  \n");
  print("    / \\  |  \\/  |/ _ \\/ ___| \n");
  print("   / _ \\ | |\\/| | | | \\___ \\ \n");
  print("  / ___ \\| |  | | |_| |___) |\n");
  print(" /_/   \\_\\_|  |_|\\___/|____/ \n");

  /*while(1)
  {
    __asm__("NOP");
  }*/

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
    }
    else if (strEql(currCommand, "stop"))
    {
      __asm__ __volatile__("cli");
      free_tasks();
    }
    else if (strEql(currCommand, "task"))
    {
      __asm__ __volatile__("sti");
      init_timer(100);

      create_task(0, (void*)task_first, (char*)4000);
      create_task(1, (void*)task_second, (char*)6000);
      create_task(2, (void*)task_third, (char*)2000);
      create_task(3, (void*)print_menu, (char*)8000); // *)task_fourth, (char*)8000);
    }
    else if (strEql(currCommand, "clear"))
    {
      clearScreen();
    }
  }
}

int kmain()
{
  init_gdt();

  isr_install();

  clearScreen();

  init_paging();

  print_menu();

  return 0;
}
