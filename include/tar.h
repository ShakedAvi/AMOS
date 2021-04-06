#ifndef TAR_H
#define TAR_H

#include "util.h"
#include "disk.h"
#include "vfs.h"
#include "paging.h"

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

typedef struct tar
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
}tar_t;

/*
  The function initializes the TAR file system.

  Input:
    None.
  Output:
    Return code (0 for success).
*/
int32 tar_init();

/*
  The function opens a new TAR file, or returns a pointer to the file if it
  already exists.

  Input:
    A file with information about the file to open.
  Output:
    Return code (0 for success).
*/
int32 tar_open(file_t* file);

/*
  The function removes a TAR file.

  Input:
    A file with information about the file to remove.
  Output:
    Return code (0 for success).
*/
int32 tar_close(file_t* file);

/*
  The function reads the content of a file.

  Input:
    A file with information about the file to read, the content's buffer and the size
    to read (in sectors).
  Output:
    Return code (0 for success).
*/
int32 tar_read(file_t* file, char* buffer, uint32 size);

/*
  The function writes the content of a file.

  Input:
    A file with information about the file to write, the content's buffer and the size
    to write (in sectors).
  Output:
    Return code (0 for success).
*/
int32 tar_write(file_t* file, char* buffer, uint32 size);

/*
  The function prints all of the TAR files.

  Input:
    None.
  Output:
    None.
*/
void tar_ls();

#endif
