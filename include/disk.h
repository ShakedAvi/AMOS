#ifndef DISK_H
#define DISK_H

#include "ahci.h"
#include "types.h"

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

/*
  Accesses The Disk via the SATA port, then reads or writes 'count' sectors (either 8 i.e 4096 bytes or 1 i.e 512 bytes)
  from sector offset 'startl:starth' into '*buf'
  Input:
    HBA_PORT *port: The port in which the SATA drive sits on, we read and write through that port
    uint32 starl/starh: sector offsets, those 2 uint32 numbers determine which sector we read from or write into
    uint32 count: Number of sectors to read\write from, if count = 8 we read 8 disk sectors (4096 bytes) and if count = 1 we read 1 disk sector (512 bytes)
    uint16 *buf: The buffer we read into (when we read)
    uint32 type: if type = 0, we access the disk for reading purposes, if type = 1, we access the disk for writing purposes.
  Output:
    1 if accessing the disk was succesful, 0 otherwise.
*/
uint8 disk_access(HBA_PORT *port, uint32 startl, uint32 starth, uint32 count, uint8 *buf, uint32 type);

/*
	Find a free command list slot
	Input:
		HBA_PORT *port: The port in which the SATA drive sits on, we read and write through that port
	Output:
		The number of the free command list slot
*/
int32 find_cmdslot(HBA_PORT *port);
#endif
