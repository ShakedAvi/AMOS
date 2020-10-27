#ifndef TASK_H
#define TASK_H

#include "paging.h"
#include "screen.h"
#include "util.h"
#include "timer.h"
#include "gdt.h"

typedef enum {
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

extern void timer_handler();

void timer_handler_main(unsigned int last_ebp, unsigned int last_esp);

void init_timer(uint32 frequency);

extern void switch_stack_and_jump(unsigned int stack, unsigned int task);

extern void switch_stack(unsigned int esp, unsigned int ebp);

void create_task(int taskIdx, void *method, char *stack);
void free_tasks();

#endif
