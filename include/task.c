#include "task.h"

static volatile int32 current_task_idx;
static task_t* current_task;
static task_t* tasks[4];

static lock_t myLock;

/*
  The function fetches-and-adds to a pointer.

  Input:
    The pointer to fetch-and-add.
  Output:
    The value of the pointer before the add.
*/
int FAA(int* ptr)
{
  int old = *ptr;
  *ptr = old + 1;
  return old;
}

/*
  The function initializes a lock.

  Input:
    The lock to initialize.
  Output:
    None.
*/
void init_lock(lock_t* lock)
{
  lock->ticket = 0;
  lock->turn = 0;
}

/*
  The function acquires a lock.

  Input:
    The lock to acquire.
  Output:
    None.
*/
void acquire(lock_t* lock)
{
  int myturn = FAA(&lock->ticket);
  while (lock->turn != myturn); // spin
}

/*
  The function releases a lock.

  Input:
    The lock to release.
  Output:
    None.
*/
void release(lock_t*lock)
{
  lock->turn += 1;
}

/*
  The function waits a fixed amount of time.

  Input:
    None.
  Output:
    None.
*/
void sleep()
{
  for (uint32 i = 0; i < 10000000; i++){}
}

/*
  The function initializes the multitasking system.

  Input:
    None.
  Output:
    None.
*/
void init_tasking()
{
  asm volatile("cli");

  init_lock(&myLock);

	asm volatile("sti");
}

/*
  The timer handler (which switches a task every tick).

  Input:
    The ebp and esp registers of the last task.
  Output:
    None.
*/
void timer_handler_main(uint32 last_ebp, uint32 last_esp)
{
    outportb(0x20, 0x20); //write the EOI. This allows another interrupt to come in later

    int32 old_task_idx = current_task_idx;
    int32 task_idx = ++current_task_idx;
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
      switch_stack_and_jump((uint32)(task_to_run->esp), (uint32)(task_to_run->task));
    }

    switch_stack(task_to_run->esp, task_to_run->ebp);

    return;
}

/*
  The function creates a new task.

  Input:
    The number of the task, it's method and a pointer to it's stack.
  Output:
    None.
*/
void create_task(int32 taskIdx, void* method, char* stack)
{
	task_t* task = (task_t*)kmalloc(sizeof(task_t));
	task->status = TASK_WAITING_FOR_START;
	task->ebp = (uint32)stack; //creates a new stack
	task->esp = (uint32)stack;
	task->task = method;

	tasks[taskIdx] = task;
}

/*
  The function frees all of the tasks.

  Input:
    None.
  Output:
    None.
*/
void free_tasks()
{
  for (int i = 0; i < 4; i++)
  {
    kfree(tasks[i]);
    tasks[i] = 0;
  }
}
