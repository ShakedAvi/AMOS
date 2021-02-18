#ifndef DISK_H
#define DISK_H

#include "ahci.h"
#include "types.h"

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

uint8 disk_access(HBA_PORT *port, uint32 startl, uint32 starth, uint32 count, uint8 *buf, uint32 type);
int32 find_cmdslot(HBA_PORT *port);
#endif
