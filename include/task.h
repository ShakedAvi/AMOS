#ifndef TASK_H
#define TASK_H

#define KERNEL_STACK_SIZE 2048

#include "paging.h"
#include "screen.h"
#include "util.h"
#include "timer.h"
#include "gdt.h"
#include "syscall.h"

extern uint32 initial_esp;

typedef enum task_status
{
	TASK_WAITING_FOR_START,
	TASK_WAITING,
	TASK_RUNNING
} task_status_t;

typedef struct task
{
	task_status_t status;
	unsigned int ebp;
	unsigned int esp;
	void *task;

	page_directory_t	*page_directory;
	uint32 kernel_stack;
} task_t;

typedef struct lock
{
	int ticket;
	int turn;
} lock_t;

/*
  The function fetches-and-adds to a pointer.

  Input:
    The pointer to fetch-and-add.
  Output:
    The value of the pointer before the add.
*/
int FAA(int* ptr);

/*
  The function initializes a lock.

  Input:
    The lock to initialize.
  Output:
    None.
*/
void init_lock(lock_t*lock);

/*
  The function acquires a lock.

  Input:
    The lock to acquire.
  Output:
    None.
*/
void acquire(lock_t*lock);

/*
  The function releases a lock.

  Input:
    The lock to release.
  Output:
    None.
*/
void release(lock_t*lock);

/*
  The function waits a fixed amount of time.

  Input:
    None.
  Output:
    None.
*/
void sleep();

/*
  The function initializes the multitasking system.

  Input:
    None.
  Output:
    None.
*/
void init_tasking();

/*
  The timer handler (which switches a task every tick).

  Input:
    The ebp and esp registers of the last task.
  Output:
    None.
*/
void timer_handler_main(unsigned int last_ebp, unsigned int last_esp);

extern void switch_stack_and_jump(unsigned int stack, unsigned int task);
extern void switch_stack(unsigned int esp, unsigned int ebp);

/*
  The function creates a new task.

  Input:
    The number of the task, it's method and a pointer to it's stack.
  Output:
    None.
*/
void create_task(int taskIdx, void *method, char *stack);

/*
  The function frees all of the tasks.

  Input:
    None.
  Output:
    None.
*/
void free_tasks();

#endif
