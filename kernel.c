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
#include "include/mouse.h"
#include "include/sound.h"
#include "include/interpreter.h"
#include "include/pci.h"
#include "include/tar.h"
#include "include/graphic.h"
#include "include/rtc.h"

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

  char currStr[200] = { 0 };
  char currCommand[10] = {0};
  char currFileName[100] = {0};

  int i = 0;

  // Testing Page Fault:
  //uint32 *ptr = (uint32*)0xA0000000;
  //uint32 do_page_fault = *ptr;

  while(exitFlag)
  {
    for (uint32 k = 0; k < 10; k++)
    {
      currCommand[k] = 0;
    }

    print("\n>> ");
    readStr(currStr);
    i = 0;
    while(i < 10 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
    {
      currCommand[i] = currStr[i];
      i++;
    }
    i++;
    currCommand[i] = 0;

    if (strEql(currCommand, "exit"))
    {
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
    else if (strEql(currCommand, "usermode"))
    {
      __asm__ __volatile__("sti");
      print("\nSwitching to User Mode (and Calling a Syscall):\n");
      jump_usermode();
    }
    else if (strEql(currCommand, "run"))
    {
      char buffer[512] = { 0 };
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      currFileName[j] = 0;
      file newFile = {0};

      strCpy(newFile.name, currFileName);

      tar_open(&newFile);
      tar_read(&newFile, buffer, 1);
      run_interpreter(buffer);
    }
    else if (strEql(currCommand, "graphic"))
    {
      init_mouse();
      print("\nMouse initialized\n\n");

      regs16_t regs;
      regs.ax = 0x0013;
      rint32(0x10, &regs);

      //draw_mouse(0, 0, 4);
      //draw_mouse(320, 200, 4);
      //clear_graphic();

      while (1)
      {
        mouse_handler();
      }
    }
    else if (strEql(currCommand, "ls"))
    {
      tar_ls();
    }
    else if (strEql(currCommand, "cat"))
    {
      char buffer[512] = { 0 };
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      currFileName[j] = 0;
      file newFile = {0};

      strCpy(newFile.name, currFileName);

      tar_open(&newFile);
      tar_read(&newFile, buffer, 1);
      print("\n");
      print(buffer);
      print("\n");
    }
    else if (strEql(currCommand, "touch"))
    {
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      print("File name: ");
      print(currFileName);
      file newFile = {0};
      strCpy(newFile.name, currFileName);
      tar_open(&newFile);
    }
    else if (strEql(currCommand, "write"))
    {
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      currFileName[j] = 0;

      /*print("\n");
      print(currFileName);
      print("\n");*/

      file newFile = {0};

      strCpy(newFile.name, currFileName);

      tar_open(&newFile);

      j = 0;
      i++;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      currFileName[j] = 0;

      print("Content: \n");
      print(currFileName);
      print("\n");

      tar_write(&newFile, currFileName, 1);
    }
    else if (strEql(currCommand, "time"))
    {
      time_t currTime;
      char printMe[20] = { 0 };
      gettime(&currTime);

      print("\n");

      switch(currTime.day_of_week)
      {
        case 1:
          print("Sunday, ");
          break;
        case 2:
          print("Monday, ");
          break;
        case 3:
          print("Tuesday, ");
          break;
        case 4:
          print("Wednesday, ");
          break;
        case 5:
          print("Thursday, ");
          break;
        case 6:
          print("Friday, ");
          break;
        case 7:
          print("Saturday, ");
          break;
      }

      itoa(currTime.day_of_month, printMe, 10);
      print(printMe);

      print("/");
      itoa(currTime.month, printMe, 10);
      print(printMe);

      print("/");
      itoa(currTime.year, printMe, 10);
      print(printMe);
      print("\n\n");

      itoa(currTime.hour, printMe, 10);
      print(printMe);

      print(":");
      itoa(currTime.minute, printMe, 10);
      print(printMe);

      print(":");
      itoa(currTime.second, printMe, 10);
      print(printMe);
      print("\n");
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

  uint32 memorySize = ((mboot_ptr->mem_lower + mboot_ptr->mem_upper) * 1024);

  init_paging(memorySize);
  print("Paging initialized\n\n");

  print("Calling PCI:\n");
  pci_init();
  print("PCI initialized\n\n");

  print("Setting up AHCI Driver:");
  HBA_MEM* abar = get_abar();
  diskPort = probe_port(abar);
  print("AHCI Driver initialized\n\n");

  /*char str[512] = { 0 };

  for (int i = 0; i < 512; i++) {
    str[i] = 'S';
  }

  disk_access(diskPort, 1, 0, 1, str, WRITE);*/

  tar_init();

  //init_keyboard();
  print("Keyboard initialized\n\n");

  init_tasking();
  print("Tasking initialized\n\n");

  init_syscalls();
  print("Syscalls initialized\n\n");

  rtc_install();
  print("Keyboard initialized\n\n");

  print_menu();

  return 0;
}
