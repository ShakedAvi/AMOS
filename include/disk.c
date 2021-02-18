#include "disk.h"

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
uint8 disk_access(HBA_PORT *port, uint32 startl, uint32 starth, uint32 count, uint8 *buf, uint32 type)
{
	port->is = (uint32) -1;	// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
	{
    return 0;
  }

	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32);	// Command FIS size
	cmdheader->w = type;	// Write from device
	cmdheader->prdtl = (uint16)((count-1)>>4) + 1;	// PRDT entries count

	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	memory_set(cmdtbl, 0, sizeof(HBA_CMD_TBL) + (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));

	// 8K bytes (16 sectors) per PRDT
  int i = 0;
	for (i = 0; i < cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint32) buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024-1;	// 8K bytes (this value should always be set to 1 less than the actual value)
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint32) buf;
	cmdtbl->prdt_entry[i].dbc = (count<<9)-1;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;

	// Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);

	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;

	cmdfis->lba0 = (uint8)startl;
	cmdfis->lba1 = (uint8)(startl>>8);
	cmdfis->lba2 = (uint8)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode

	cmdfis->lba3 = (uint8)(startl>>24);
	cmdfis->lba4 = (uint8)starth;
	cmdfis->lba5 = (uint8)(starth>>8);

	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;

	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		print("Port is hung\n");
		return 0;
	}

	port->ci = 1<<slot;	// Issue command

	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0)
		{
      break;
    }
		if (port->is & HBA_PXIS_TFES)	// Task file error
		{
			print("Read disk error #1\n");
			return 0;
		}
	}

	// Check again
	if (port->is & HBA_PXIS_TFES)
	{
		print("Read disk error #2\n");
		return 0;
	}

	return 1;
}

// Find a free command list slot
int find_cmdslot(HBA_PORT *port)
{
	// If not set in SACT and CI, the slot is free
	uint32 slots = (port->sact | port->ci);
  //int32 num_of_slots= (abar->cap & 0x0f00) >> 8; // Bit 8-12
	for (int32 i = 0; i < 32; i++) //Add define for num_of_slots
	{
		if ((slots & 1) == 0)
		{
      return i;
    }
		slots >>= 1;
	}
	print("Cannot find free command list entry\n");
	return -1;
}
