#include "tar.h"

static HBA_PORT* diskPort;
static uint32 currFile;
struct tar_t * archive;

fs ustar;
vnode *ustar_root;

static int tar_init()
{
  fs ustar =
  {
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

  currFile = 0;
  ustar_root = (vnode*)kmalloc(sizeof(struct vnode));
  ustar_root->fs = &ustar;
  vfs_install(&ustar);

  archive = 0;

  char ptr[512] = {0};
  uint64 v = 1024 * currFile + 1;
  uint32 low = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 high = (uint32)(v & 0xFFFFFFFFLL);
  disk_access(diskPort, low, high, 1, ptr, READ);

  while(!memory_compare(ptr + 257, "ustar", 5))
  {
      memory_copy(ptr, ustar.files[currFile], 100);
      currFile++;
      //ptr += (((filesize + 511) / 512) + 1) * 512;
      v = 1024 * currFile + 1;
      low = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
      high = (uint32)(v & 0xFFFFFFFFLL);
      disk_access(diskPort, low, high, 1, ptr, READ);
  }
  return 0;
}

int32 tar_open(file* file)
{
  //Searching for file
  for (int i = 0; i <= currFile; i++)
  {
    if(strEql(ustar.files[i]->name, file->name))
    {
      file = ustar.files[i];
      return 0;
    }
  }

  //Creating new file and saving it to disk
  struct tar_t fileHeader = {0};
  strCpy(fileHeader.name, file->name);
  memory_set(fileHeader.block, 0, 512);
  uint64 v = 1024 * currFile + 1;
  uint32 low = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 high = (uint32)(v & 0xFFFFFFFFLL);
  disk_access(diskPort, low, high, 2, (char*)&fileHeader, WRITE);

  struct file* newFile = (struct file*)kmalloc(sizeof(struct file*));
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
  uint64 v = 1024 * file->vnode->ref + 1 + 512;
  uint32 low = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 high = (uint32)(v & 0xFFFFFFFFLL);
  disk_access(diskPort, low, high, size, buffer, READ);
  return 0;
}

int32 tar_write(file* file, char* buffer, uint32 size)
{
  uint64 v = 1024 * file->vnode->ref + 1 + 512;
  uint32 low = (uint32)((v & 0xFFFFFFFF00000000LL) >> 32);
  uint32 high = (uint32)(v & 0xFFFFFFFFLL);
  disk_access(diskPort, low, high, size, buffer, WRITE);
  return 0;
}

void tar_ls()
{
  char fileNumber[10];
  for (int i = 0; i <= currFile; i++)
  {
    itoa(ustar.files[i]->ref, fileNumber, 10);
    print(fileNumber);
    print(": ");
    print(ustar.files[i]->name);
    print("\n");
  }
}

/*
int tar_lookup(unsigned char *archive, char *filename, char **out)
{
    unsigned char *ptr = archive;

    while (!memory_compare(ptr + 257, "ustar", 5))
    {
        int filesize = oct2bin(ptr + 0x7c, 11);
        if (!memory_compare(ptr, filename, strlen(filename) + 1)) {
            *out = ptr + 512;
            return filesize;
        }
        ptr += (((filesize + 511) / 512) + 1) * 512;
    }
    return 0;
}

/*
int32 tar_read(const int32 file_descriptor, struct tar_t **archive, const char verbosity)
{
  if (file_descriptor < 0)
  {
    print("Error: Invalid File Descriptor");
  }

  if(!archive || *archive)
  {
    print("Error: Invalid Archive");
  }



}

int32 tar_write(const int32 file_descriptor, struct tar_t ** archive, const uint32 filecount, const char *files, const char verbosity)
{
  if (file_descriptor < 0)
  {
    print("Error: Invalid File Descriptor");
  }

  if(!archive || *archive)
  {
    print("Error: Invalid Archive");
  }


}

int32 read_size(const int32 file_descriptor, char* buffer, int32 size)
{
  return 0;
}

int32 write_size(const int32 file_descriptor, char* buffer, int32 size)
{
  return 0;
}*/
