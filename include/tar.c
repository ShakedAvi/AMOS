#include "tar.h"

uint32 currFile;
struct tar_t* archive;

fs ustar = {
    .name  = "ustar",
    .init  = tar_init,

    .fops =
        {
        .open    = tar_open,
        .read    = tar_read,
        .write   = tar_write,
        .close   = tar_close,
        .ls = tar_ls,
    },
};
vnode *ustar_root;

int tar_init()
{
  currFile = 0;
  ustar_root = (vnode*)kmalloc(sizeof(struct vnode));
  ustar_root->fs = &ustar;
  vfs_install(&ustar);

  archive = 0;

  char ptr[512] = { 0 };
  uint64 v = 1024 * currFile;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  /*char printMe[10] = { 0 };
  itoa(low, printMe, 16);
  print("Low: ");
  print(printMe);
  print("\n");

  itoa(high, printMe, 16);
  print("High: ");
  print(printMe);
  print("\n");*/

  char fileNumber[10];

  disk_access(diskPort, low, high, 1, ptr, READ);

  while(!memory_compare(ptr + 257, "ustar", 5))
  {
      ustar.files[currFile] = (file*)kmalloc(sizeof(file));
      memory_copy(ptr, ustar.files[currFile]->name, 100);
      ustar.files[currFile]->vnode = (vnode*)kmalloc(sizeof(vnode));
      ustar.files[currFile]->vnode->ref = currFile;

      currFile++;
      //ptr += (((filesize + 511) / 512) + 1) * 512;

      for (int k = 0; k < 512; k++)
      {
        ptr[k] = 0;
      }
      v = 1024 * currFile;
      high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
      low = (uint32)(v & 0xFFFFFFFFLL);
      disk_access(diskPort, low, high, 1, ptr, READ);
  }

  return 0;
}

int32 tar_open(file* file)
{
  char fileNumber[10] = { 0 };

  //Searching for file
  for (int i = 0; i < currFile; i++)
  {
    if(strEql(ustar.files[i]->name, file->name))
    {
      strCpy(file->name, ustar.files[i]->name);
      file->vnode->ref = ustar.files[i]->vnode->ref;
      return 0;
    }
  }

  //Creating new file and saving it to disk
  struct tar_t fileHeader = {0};
  strCpy(fileHeader.name, file->name);
  strCpy(fileHeader.ustar, "ustar\000");
  memory_set(fileHeader.block, 0, 512);
  uint64 v = 1024 * currFile;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);
  char* str = (char*)&fileHeader;
  /*print("\n");
  for (uint32 i = 0; i < 1024; i++) {
    printch(str[i]);
  }
  print("\n");*/
  disk_access(diskPort, low, high, 2, str, WRITE);

  struct file* newFile = (struct file*)kmalloc(sizeof(struct file));
  newFile->vnode = (vnode*)kmalloc(sizeof(vnode));
  newFile->vnode->fs = &ustar;
  newFile->vnode->ref = currFile;

  strCpy(newFile->name, file->name);
  ustar.files[currFile] = newFile;
  currFile++;
  file = newFile;
  return 0;
}

int32 tar_close(file* file)
{
  kfree(file);
  return 0;
}

int32 tar_read(file* file, char* buffer, uint32 size)
{
  // Splitting the file address into high and low dwords
  uint64 v = (1024 * file->vnode->ref + 512) / 512; //file->vnode->ref + 512;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  disk_access(diskPort, low, high, size, buffer, READ);
  return 0;
}

int32 tar_write(file* file, char* buffer, uint32 size)
{
  char fileNumber[10] = { 0 };

  uint64 v = (1024 * file->vnode->ref + 512) / 512; //file->vnode->ref + 512;
  uint32 high = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 low = (uint32)(v & 0xFFFFFFFFLL);

  disk_access(diskPort, low, high, size, buffer, WRITE);
  return 0;
}

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
