#ifndef PROCFS_H
#define PROCFS_H

#include "util.h"
#include "types.h"
#include "kheap.h"
#include "vfs.h"
#include "rtc.h"

typedef struct proc
{
	char name[100];
	int pid;
	int permissions;
}proc_t;

static proc_t* currentprocs[5];

/*
  The function initializes the processes' file system.

  Input:
    None.
  Output:
    Return code (0 for success).
*/
int procfs_init();

/*
  The function opens a new process file.

  Input:
    A file with information about the process to open.
  Output:
    Return code (0 for success).
*/
int procfs_open(file_t *file);

/*
  The function removes a process file.

  Input:
    A file with information about the process to close.
  Output:
    Return code (0 for success).
*/
int procfs_close(file_t *file);

/*
  The function prints all of the processes files.

  Input:
    None.
  Output:
    None.
*/
void procfs_ls();

#endif
