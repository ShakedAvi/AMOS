#ifndef PROCFS_H
#define PROCFS_H

#include "util.h"
#include "types.h"
#include "kheap.h"
#include "vfs.h"

typedef struct proc
{
	char* name;
	int pid;
	int priority;
	char* creationDate;
	int permissions;
}proc;

static proc* currentprocs[5];

void createproc(char name[5]);
void deleteproc(int pid);

void printAllprocs(proc* curr);
void printProperties(int pid);

static int procfs_init();
int procfs_open(file *file);
uint32 procfs_read(file *file, void *buf, uint32 size);
int procfs_close(file *file);

#endif
