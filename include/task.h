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

typedef enum
{
	TASK_WAITING_FOR_START,
	TASK_WAITING,
	TASK_RUNNING
} task_status_t;

typedef struct
{
	task_status_t status;
	unsigned int ebp;
	unsigned int esp;
	void *task;

	page_directory_t	*page_directory;
	uint32 kernel_stack;
} task_t;

typedef struct
{
	int ticket;
	int turn;
} lock_t;

int FAA(int* ptr);

void init_lock(lock_t*lock);

void acquire(lock_t*lock);

void release(lock_t*lock);

void sleep();

void init_tasking();

void timer_handler_main(unsigned int last_ebp, unsigned int last_esp);

extern void switch_stack_and_jump(unsigned int stack, unsigned int task);

extern void switch_stack(unsigned int esp, unsigned int ebp);

void create_task(int taskIdx, void *method, char *stack);
void free_tasks();

void switch_to_user_mode();

#endif
