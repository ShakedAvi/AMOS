#include "task.h"

static volatile int current_task_idx;
static task_t* current_task;
static task_t* tasks[4];

static lock_t myLock;

int FAA(int* ptr)
{
  int old = *ptr;
  *ptr = old + 1;
  return old;
}

void init_lock(lock_t*lock)
{
  lock->ticket = 0;
  lock->turn = 0;
}

void acquire(lock_t*lock)
{
  int myturn = FAA(&lock->ticket);
  while (lock->turn != myturn); // spin
}

void release(lock_t*lock)
{
  lock->turn += 1;
}

void sleep()
{
  for (int i = 0; i < 1000000; i++)
  {
    __asm__("NOP");
  }
}

void init_tasking()
{
  asm volatile("cli");

  init_lock(&myLock);

	asm volatile("sti");
}

void timer_handler_main(unsigned int last_ebp, unsigned int last_esp)
{
    outportb(0x20, 0x20); //write the EOI. This allows another interrupt to come in later

    int old_task_idx = current_task_idx;
    int task_idx = ++current_task_idx;
    task_idx %= 4;
    current_task_idx = task_idx;

    task_t *old_task = tasks[old_task_idx];

    if (old_task->status == TASK_RUNNING)
    {
      old_task->ebp = last_ebp; // save the status of the task
      old_task->esp = last_esp; // all the registers are saved on the stack, so its the only important value to get
    }

    task_t *task_to_run = tasks[current_task_idx];

    if (task_to_run->status == TASK_WAITING_FOR_START)
    {
      task_to_run->status = TASK_RUNNING;
      switch_stack_and_jump((unsigned int)(task_to_run->esp), (unsigned int)(task_to_run->task));
    }

    switch_stack(task_to_run->esp, task_to_run->ebp);

    return;
}

void create_task(int taskIdx, void *method, char *stack)
{
	task_t* task = (task_t*)kmalloc(sizeof(task_t));
	task->status = TASK_WAITING_FOR_START;
	task->ebp = (unsigned int)stack; //creates a new stack
	task->esp = (unsigned int)stack;
	task->task = method;

	tasks[taskIdx] = task;
}

void free_tasks()
{
  for (int i = 0; i < 4; i++)
  {
    kfree(tasks[i]);
  }
}

void switch_to_user_mode()
{
   set_kernel_stack(initial_esp + KERNEL_STACK_SIZE);
   // Set up a stack structure for switching to user mode.

   asm volatile("  \
     mov $0x23, %ax; \
     mov %ax, %ds; \
     mov %ax, %es; \
     mov %ax, %fs; \
     mov %ax, %gs; \
                   \
     mov %esp, %eax; \
     pushl $0x23; \
     pushl %eax; \
     pushf; \
     pushl $0x1B; \
     push $1f; \
     iret; \
   1: \
     int $3 \
     ");

     //print("hello");
     //syscall_print("Hi!\n");
     //int a;
     //char* p1 = "hello";
     //asm volatile("int $0x80" : "=a" (a) : "0" (0), "b" ((int)p1));
     while(1);
}
