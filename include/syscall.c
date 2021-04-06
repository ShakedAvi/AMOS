#include "syscall.h"

// Example syscall
void f1()
{
  print("Welcome to User Mode Agent!\n");
  play_mission_impossible();
}

// Defining our example syscall
DEFN_SYSCALL0(f1, 0)

void syscall_handler(registers32_t *regs);

static void* syscalls[1] =
{
   &f1
};
uint32 num_syscalls = 1;

/*
  The function initializes syscalls.

  Input:
    None.
  Output:
    None.
*/
void init_syscalls()
{
    register_interrupt_handler(0x80, &syscall_handler);
}

/*
  The handler for all of the syscalls.

  Input:
    The syscall caller's registers.
  Output:
    None.
*/
void syscall_handler(registers32_t* regs)
{
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
