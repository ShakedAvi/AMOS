#ifndef TAR_H
#define TAR_H

#include "util.h"
#include "disk.h"
#include "vfs.h"

HBA_PORT* diskPort;

#define READ 0
#define WRITE 1

#define REGULAR          0
#define NORMAL          '0'
#define HARDLINK        '1'
#define SYMLINK         '2'
#define CHAR            '3'
#define BLOCK           '4'
#define DIRECTORY       '5'
#define FIFO            '6'
#define CONTIGUOUS      '7'

struct tar_t
{
  char name[100];             // file name
  char mode[8];               // permissions
  char uid[8];                // user id (octal)
  char gid[8];                // group id (octal)
  char size[12];              // size (octal)
  char mtime[12];             // modification time (octal)
  char check[8];              // sum of unsigned characters in block, with spaces in the check field while calculation is done (octal)
  char link;                  // link indicator
  char link_name[100];        // name of linked file
  char ustar[8];              // ustar\000
  char owner[32];             // user name (string)
  char group[32];             // group name (string)
  char major[8];              // device major number
  char minor[8];              // device minor number
  char prefix[155];

  char block[512];            // raw memory (500 octets of actual data, padded to 1 block)
};
struct tar_t * archive;

int tar_init();
int32 tar_open(file* file);
int32 tar_close(file* file);
int32 tar_read(file* file, char* buffer, uint32 size);
int32 tar_write(file* file, char* buffer, uint32 size);
void tar_ls();
//int tar_lookup(unsigned char *archive, char *filename, char **out);
//int32 tar_read(const int32 file_descriptor, struct tar_t **archive, const char verbosity);
//int32 tar_write(const int32 file_descriptor, struct tar_t **archive, const uint32 filecount, const char *files, const char verbosity);
//int32 read_size(const int32 file_descriptor, char* buffer, int32 size);
//int32 write_size(const int32 file_descriptor, char* buffer, int32 size);
#endif
