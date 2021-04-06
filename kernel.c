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
#include "include/chess.h"
#include "include/syscall.h"
#include "include/mouse.h"
#include "include/sound.h"
#include "include/interpreter.h"
#include "include/pci.h"
#include "include/tar.h"
#include "include/procFS.h"
#include "include/graphic.h"
#include "include/rtc.h"

#define TAR_FS 0
#define PROC_FS 1

// Jumping to user mode asm function
extern void jump_usermode();

// The current file system
int currFS = TAR_FS;

// The initial stack pointer
uint32 initial_esp;

// Lock for tasks
static lock_t myLock;

// Tasks' Functions Examples:
void task_first()
{
  while (1)
	{
    acquire(&myLock);
		print("First!\n");
    release(&myLock);
	}
}

void task_second()
{
  while (1)
	{
    acquire(&myLock);
		print("Second!\n");
    release(&myLock);
  }
}

void task_third()
{
  while (1)
	{
    acquire(&myLock);
		print("Third!\n");
    release(&myLock);
	}
}

void task_fourth()
{
  while (1)
	{
    acquire(&myLock);
		print("Fourth!\n");
    release(&myLock);
	}
}
// End Of Tasks' Examples

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

    if (strEql(currCommand, "exit")) // Exit From The Operating System
    {
      print("\nGoodbye!\n");
      exitFlag = 0;
    }
    else if (strEql(currCommand, "help")) // Help Command
    {
      print("\n\n    Welcome to AMOS - Aviram & Malinsky Operating System\n\n");
      print("exit                        - Exit From The Operating System\n");
      print("help                        - Shows This Help Screen\n");
      print("task                        - Start Multitasking Example\n");
      print("stop                        - Stop Multitasking Example\n");
      print("rmt                         - Remove Tasks' Files\n");
      print("chess                       - Start Chess Example\n");
      print("clear                       - Clear The Screen\n");
      print("usermode                    - Start User Mode Example\n");
      print("run <File Name>             - Start Interpreter (And Run <File Name>)\n");
      print("graphic                     - Start Graphic Example\n");
      print("cd tar/proc                 - Switch Between File Systems\n");
      print("ls                          - Show All Files\n");
      print("cat <File Name>             - Print The Content of <File Name>\n");
      print("touch <File Name>           - Create New File (<File Name>)\n");
      print("rm <File Name>              - Remove a File (<File Name>)\n");
      print("write <File Name> <Content> - Write <Content> To a File (<File Name>)\n");
      print("time                        - Print Time and Date From Real Time Clock\n");
      print("\n\n");
    }
    else if (strEql(currCommand, "task")) // Start Multitasking Example
    {
      __asm__ __volatile__("sti");
      init_timer(100);

      file_t newFile1 = {0};
      strCpy(newFile1.name, "Menu Process");
      newFile1.flags = 0;
      procfs_open(&newFile1);
      create_task(0, (void*)print_menu, (char*)initial_esp);

      file_t newFile2 = {0};
      strCpy(newFile2.name, "Second Process");
      newFile2.flags = 0;
      procfs_open(&newFile2);
      create_task(1, (void*)task_second, (char*)6000);

      file_t newFile3 = {0};
      strCpy(newFile3.name, "Third Process");
      newFile3.flags = 0;
      procfs_open(&newFile3);
      create_task(2, (void*)task_third, (char*)2000);

      file_t newFile4 = {0};
      strCpy(newFile4.name, "Fourth Process");
      newFile4.flags = 0;
      procfs_open(&newFile4);
      create_task(3, (void*)task_fourth, (char*)8000);
    }
    else if (strEql(currCommand, "stop")) // Stop Multitasking Example
    {
      __asm__ __volatile__("cli");
      free_tasks();
    }
    else if (strEql(currCommand, "rmt")) // Remove Tasks' Files
    {
      file_t newFile1 = {0};
      strCpy(newFile1.name, "Menu Process");
      procfs_close(&newFile1);

      file_t newFile2 = {0};
      strCpy(newFile2.name, "Second Process");
      procfs_close(&newFile2);

      file_t newFile3 = {0};
      strCpy(newFile3.name, "Third Process");
      procfs_close(&newFile3);

      file_t newFile4 = {0};
      strCpy(newFile4.name, "Fourth Process");
      procfs_close(&newFile4);
    }
    else if(strEql(currCommand, "chess")) // Start Chess Example
    {
      start_game();
    }
    else if (strEql(currCommand, "clear")) // Clear The Screen
    {
      clearScreen();
    }
    else if (strEql(currCommand, "usermode")) // Start User Mode Example
    {
      __asm__ __volatile__("sti");
      print("\nSwitching to User Mode (and Calling a Syscall):\n");
      jump_usermode();
    }
    else if (strEql(currCommand, "run")) // Start Interpreter
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
      file_t newFile = {0};

      strCpy(newFile.name, currFileName);

      tar_open(&newFile);
      tar_read(&newFile, buffer, 1);
      run_interpreter(buffer);
    }
    else if (strEql(currCommand, "graphic")) // Start Graphic Example
    {
      init_mouse();
      print("\nMouse initialized\n\n");

      registers16_t regs;
      regs.ax = 0x0013;
      rint32(0x10, &regs);

      while (1)
      {
        mouse_handler();
      }
    }
    else if (strEql(currCommand, "cd")) // Switch Between File Systems
    {
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      currFileName[j] = 0;
      if (strEql(currFileName, "tar"))
      {
        currFS = TAR_FS;
      }
      else if (strEql(currFileName, "proc"))
      {
        currFS = PROC_FS;
      }
      else
      {
          print("\nError! No Valid File System Name!\n");
      }
    }
    else if (strEql(currCommand, "ls")) // Show All Files
    {
      if(currFS == TAR_FS)
      {
        tar_ls();
      }
      else
      {
        procfs_ls();
      }
    }
    else if (strEql(currCommand, "cat")) // Print File's Content
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
      file_t newFile = {0};

      strCpy(newFile.name, currFileName);

      if(currFS == TAR_FS)
      {
        tar_open(&newFile);
        tar_read(&newFile, buffer, 1);
        print("\n");
        print(buffer);
        print("\n");
      }
      else
      {
        print("\nCannot print process files' data without 'ls' command!\n");
      }
    }
    else if (strEql(currCommand, "touch")) // Create New File
    {
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      file_t newFile = {0};
      strCpy(newFile.name, currFileName);

      if(currFS == TAR_FS)
      {
        tar_open(&newFile);
      }
      else
      {
        print("\nCannot open process files without 'task' command!\n");
      }
    }
    else if (strEql(currCommand, "rm")) // Remove a File
    {
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      file_t newFile = {0};
      strCpy(newFile.name, currFileName);

      if(currFS == TAR_FS)
      {
        tar_open(&newFile);
        tar_close(&newFile);
      }
      else
      {
        print("\nCannot remove process files without 'stop' command!\n");
      }
    }
    else if (strEql(currCommand, "write")) // Write To a New/an Existing File
    {
      int j = 0;
      while(j < 100 && currStr[i] != '\0' && currStr[i] != ' ' && currStr[i] != '\n')
      {
        currFileName[j] = currStr[i];
        i++;
        j++;
      }
      currFileName[j] = 0;

      file_t newFile = {0};

      strCpy(newFile.name, currFileName);

      if(currFS == TAR_FS)
      {
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

        tar_write(&newFile, currFileName, 1);
      }
      else
      {
        print("\nCannot write to process files!\n");
      }
    }
    else if (strEql(currCommand, "time")) // Print Time and Date From Real Time Clock
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

  tar_init();
  print("Tar File System initialized\n\n");
  procfs_init();
  print("Proc File System initialized\n\n");

  init_tasking();
  print("Tasking initialized\n\n");

  init_syscalls();
  print("Syscalls initialized\n\n");

  init_rtc();
  print("Real Time Clock initialized\n\n");

  print_menu();

  return 0;
}
