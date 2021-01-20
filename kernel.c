#include "include/types.h"
#include "include/multiboot.h"
#include "include/string.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/isr.h"
#include "include/timer.h"
#include "include/paging.h"
#include "include/task.h"
#include "include/gdt.h"
#include "include/vfs.h"
#include "include/usermode.h"
#include "include/chess.h"
#include "include/syscall.h"

uint32 initial_esp;

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
  clearScreen();

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

      create_task(0, (void*)print_menu, (char*)initial_esp);
      //create_task(0, (void*)task_first, (char*)4000);
      create_task(1, (void*)task_second, (char*)6000);
      create_task(2, (void*)task_third, (char*)2000);
      create_task(3, (void*)task_fourth, (char*)8000);
    }
    else if(strEql(currCommand, "chess"))
    {
      start_game();
    }
    else if (strEql(currCommand, "clear"))
    {
      clearScreen();
    }
    else if (strEql(currCommand, "syscall"))
    {
      __asm__ __volatile__("sti");
      syscall_f1();
    }
  }
}

int kmain(struct multiboot *mboot_ptr, uint32 initial_stack)
{
  initial_esp = initial_stack;

  isr_install();
  print("IDT initialized\n\n");
  init_gdt();
  print("GDT initialized\n\n");

  uint32 memorySize = ((mboot_ptr->mem_lower + mboot_ptr->mem_upper) * 1024); //Bytes

  init_paging(memorySize);
  print("Paging initialized\n\n");

  init_keyboard();
  print("Keyboard initialized\n\n");

  init_tasking();
  print("Tasking initialized\n\n");

  init_syscalls();
  print("Syscalls initialized\n\n");

  print("Calling Syscall:\n");
  syscall_f1();
  print("\n\n");

  print("Switching to User Mode (and Raising Breakpoint Interrunpt):\n");
  switch_to_user_mode();

  print_menu();

  return 0;
}
