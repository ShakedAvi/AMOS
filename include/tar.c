#include "tar.h"

uint32 currFile;

fs_t ustar = {
    .name  = "ustar"
};

/*
  The function initializes the TAR file system.

  Input:
    None.
  Output:
    Return code (0 for success).
*/
int32 tar_init()
{
  currFile = 0;

  char ptr[512] = { 0 };
  uint64 v = 1024 * currFile;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  char fileNumber[10] = { 0 };

  disk_access(diskPort, low, high, 1, ptr, READ);

  while(!memory_compare(ptr + 257, "ustar", 5))
  {
      ustar.files[currFile] = (file_t*)kmalloc(sizeof(file_t));
      memory_copy(ptr, ustar.files[currFile]->name, 100);
      ustar.files[currFile]->vnode = (vnode_t*)kmalloc(sizeof(vnode_t));
      ustar.files[currFile]->vnode->ref = currFile;

      currFile++;

      for (int k = 0; k < 512; k++)
      {
        ptr[k] = 0;
      }
      v = (1024 * currFile) / 512;
      high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
      low = (uint32)(v & 0xFFFFFFFFLL);
      disk_access(diskPort, low, high, 1, ptr, READ);
  }

  return 0;
}

/*
  The function opens a new TAR file, or returns a pointer to the file if it
  already exists.

  Input:
    A file with information about the file to open.
  Output:
    Return code (0 for success).
*/
int32 tar_open(file_t* file)
{
  char fileNumber[10] = { 0 };

  //Searching for file
  for (int i = 0; i < currFile; i++)
  {
    if(strEql(ustar.files[i]->name, file->name))
    {
      file->vnode->ref = ustar.files[i]->vnode->ref;
      return 0;
    }
  }

  //Creating new file and saving it to disk
  tar_t fileHeader = {0};
  strCpy(fileHeader.name, file->name);
  strCpy(fileHeader.ustar, "ustar\000");
  memory_set(fileHeader.block, 0, 512);
  uint64 v = (1024 * currFile) / 512;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);
  char* str = (char*)&fileHeader;

  disk_access(diskPort, low, high, 2, str, WRITE);

  file_t* newFile = (file_t*)kmalloc(sizeof(file_t));
  newFile->vnode = (vnode_t*)kmalloc(sizeof(vnode_t));
  newFile->vnode->fs = &ustar;
  newFile->vnode->ref = currFile;

  strCpy(newFile->name, file->name);
  ustar.files[currFile] = newFile;
  currFile++;
  file->vnode->ref = newFile->vnode->ref;
  return 0;
}

/*
  The function removes a TAR file.

  Input:
    A file with information about the file to remove.
  Output:
    Return code (0 for success).
*/
int32 tar_close(file_t* file)
{
  currFile--;

  uint64 v = (1024 * currFile) / 512;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  char buffer[1024] = { 0 };
  disk_access(diskPort, low, high, 2, buffer, READ);

  v = (1024 * file->vnode->ref) / 512;
  high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  low = (uint32)(v & 0xFFFFFFFFLL);

  disk_access(diskPort, low, high, 2, buffer, WRITE);

  memory_set(buffer, 0, 1024);

  v = (1024 * currFile) / 512;
  high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  low = (uint32)(v & 0xFFFFFFFFLL);

  disk_access(diskPort, low, high, 2, buffer, WRITE);

  ustar.files[currFile]->vnode->ref = file->vnode->ref;
  ustar.files[file->vnode->ref] = ustar.files[currFile];

  kfree(file);
  ustar.files[currFile] = 0;

  return 0;
}

/*
  The function reads the content of a file.

  Input:
    A file with information about the file to read, the content's buffer and the size
    to read (in sectors).
  Output:
    Return code (0 for success).
*/
int32 tar_read(file_t* file, char* buffer, uint32 size)
{
  // Splitting the file address into high and low dwords
  uint64 v = (1024 * file->vnode->ref + 512) / 512;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  disk_access(diskPort, low, high, size, buffer, READ);
  return 0;
}

/*
  The function writes the content of a file.

  Input:
    A file with information about the file to write, the content's buffer and the size
    to write (in sectors).
  Output:
    Return code (0 for success).
*/
int32 tar_write(file_t* file, char* buffer, uint32 size)
{
  char fileNumber[10] = { 0 };

  uint64 v = (1024 * file->vnode->ref + 512) / 512;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  disk_access(diskPort, low, high, size, buffer, WRITE);
  return 0;
}

/*
  The function prints all of the TAR files.

  Input:
    None.
  Output:
    None.
*/
void tar_ls()
{
  print("\n");
  char fileNumber[10];
  for (int i = 0; i < currFile; i++)
  {
    itoa(ustar.files[i]->vnode->ref + 1, fileNumber, 10);
    print(fileNumber);
    print(": ");
    print(ustar.files[i]->name);
    print("\n");
  }
}
