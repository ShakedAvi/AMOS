#include "syscall.h"

void f1()
{
  print("Welcome to User Mode Agent!\n");
  play_mission_impossible();
}

//DEFN_SYSCALL1(print, 0, char*)
DEFN_SYSCALL0(f1, 0)
DEFN_SYSCALL3(create_task, 1, int, void*, char*)
DEFN_SYSCALL0(free_tasks, 2)

static void syscall_handler(registers_t *regs);

static void *syscalls[3] =
{
   &f1,
   &create_task,
   &free_tasks,
   //&open_file,
   //&read_file,
   //&write_file,
   //&close_file,
};
uint32 num_syscalls = 3;

void init_syscalls()
{
    register_interrupt_handler(0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
   //if (regs->eax >= num_syscalls)
    //  return;

   void *location = syscalls[0];

   int ret = 0;
   asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
   regs->eax = ret;
}
