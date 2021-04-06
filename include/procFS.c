#include "procFS.h"

static proc_t* currentprocs[5];
static int currPID = 0;

fs_t procfs =
{
    .name  = "procfs"
};

/*
  The function initializes the processes' file system.

  Input:
    None.
  Output:
    Return code (0 for success).
*/
int procfs_init()
{
    proc_t* rootProc = (proc_t*)kmalloc(sizeof(proc_t));
    strCpy(rootProc->name, "root");
    rootProc->permissions = 0;
    rootProc->pid = currPID;

    currentprocs[currPID++] = rootProc;

    return 0;
}

/*
  The function opens a new process file.

  Input:
    A file with information about the process to open.
  Output:
    Return code (0 for success).
*/
int procfs_open(file_t* file)
{
	proc_t* newProc = (proc_t*)kmalloc(sizeof(proc_t));
  strCpy(newProc->name, (char*)(file->name));
  newProc->pid = currPID;
  newProc->permissions = file->flags;
	currentprocs[currPID] = newProc;
  print("\n");
  print(currentprocs[currPID]->name);
  print("\n");

	file_t* newPFile = (file_t*)kmalloc(sizeof(file_t));

  strCpy(newPFile->name, file->name);
  newPFile->vnode = (vnode_t*)kmalloc(sizeof(vnode_t));
	newPFile->vnode->fs = &procfs;
  newPFile->vnode->ref = currPID;
	newPFile->flags = file->flags; // permissions

	procfs.files[currPID] = newPFile;
  file->vnode->ref = currPID;

  currPID++;
  return 0;
}

/*
  The function removes a process file.

  Input:
    A file with information about the process to close.
  Output:
    Return code (0 for success).
*/
int procfs_close(file_t *file)
{
  int i = 0;
  for (i = 0; i < currPID; i++)
  {
    if(strEql(file->name, currentprocs[i]->name))
    {
      file->vnode->ref = i;
      break;
    }
  }

  if(i == currPID)
  {
    print("\nError! Process was not found!\n");
    return -1;
  }

	kfree(currentprocs[(int)(file->vnode->ref)]->name);
	kfree(currentprocs[(int)(file->vnode->ref)]);
  currentprocs[(int)(file->vnode->ref)] = 0;
	kfree(procfs.files[(int)(file->vnode->ref)]->vnode);
	kfree(procfs.files[(int)(file->vnode->ref)]);
  procfs.files[(int)file->vnode->ref] = 0;

  if(i == currPID - 1)
  {
    currPID = 1;
  }

  return 0;
}

/*
  The function prints all of the processes files.

  Input:
    None.
  Output:
    None.
*/
void procfs_ls()
{
  char printMe[20] = { 0 };

  print("\nPID:");
  print("    ");
  print("Name: ");
  print("    ");
  print("Permissions:\n\n");

  for(int i = 0; i < currPID; i++)
  {
    itoa(currentprocs[i]->pid, printMe, 10);
    print(printMe);
    print("       ");
    print(currentprocs[i]->name);
    print("      ");
    if(currentprocs[i]->permissions)
    {
      print("User Mode\n");
    }
    else
    {
      print("Kernel Mode\n");
    }
  }
}
