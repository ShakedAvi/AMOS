#include "ahci.h"

/*
	reconfigurates all the necessary AHCI memory spaces
	Input:
		HBA_PORT *port: the port in which the SATA Device is connected into
		int portno: The number of the port (1 -> 32)
	Output:
		None
*/
void port_rebase(HBA_PORT *port, int portno)
{
	stop_cmd(port);	// Stop command engine

	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = AHCI_BASE + (portno<<10);
	port->clbu = 0;
	memory_set((void*)(port->clb), 0, 1024);

	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE + (32<<10) + (portno<<8);
	port->fbu = 0;
	memory_set((void*)(port->fb), 0, 256);

	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		memory_set((void*)cmdheader[i].ctba, 0, 256);
	}

	start_cmd(port);	// Start command engine
}

/*
	Starts the Command Engine
	Input:
		HBA_PORT *port: A Pointer to a struct containing all the ports and registers recieved from the ABAR
	Output:
		None
*/
void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	while (port->cmd & HBA_PXCMD_CR)
		;

	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PXCMD_FRE;
	port->cmd |= HBA_PXCMD_ST;
}

/*
	Stops the Command Engine
	Input:
		HBA_PORT *port: A Pointer to a struct containing all the ports and registers recieved from the ABAR
	Output:
		None
*/
void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PXCMD_ST;

	// Clear FRE (bit4)
	port->cmd &= ~HBA_PXCMD_FRE;

	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PXCMD_FR)
			continue;
		if (port->cmd & HBA_PXCMD_CR)
			continue;
		break;
	}

}

/*
	Checks what device is attached to each port
	Input:
		HBA_PORT *abar: The AHCI Base Memory Register ( Contains The Registers and Memories of the AHCI Controller)
	Output:
		The registers of the port that the SATA Device is connected to, saved as a struct called HBA_PORT
*/
HBA_PORT* probe_port(HBA_MEM *abar)
{
	// Searches for a disk in implemented ports
	HBA_PORT* sata_port = 0;
	uint32 pi = abar->pi;
	int32 port = 0;
	while (port < 32)
	{
		if (pi & 1)
		{
			int32 dt = check_type(&abar->ports[port]);
			if (dt == AHCI_DEV_SATA)
			{
				//print("SATA drive found at port %d\n", i);
				print("SATA drive found \n");
				sata_port = &abar->ports[port];
				port_rebase(sata_port, port);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				//print("SATAPI drive found at port %d\n", i);
				print("SATAPI drive found \n");
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				//print("SEMB drive found at port %d\n", i);
				print("SEMB drive found \n");
			}
			else if (dt == AHCI_DEV_PM)
			{
				//print("PM drive found at port %d\n", i);
				print("PM drive found \n");
			}
			else
			{
				//print("No drive found at port %d\n", i);
				print("No drive found \n");
			}
		}

		pi >>= 1;
		port++;
	}
	return sata_port;
}

/*
	Checks the type of the device attached to the port
	Input:
		HBA_PORT *port: A Pointer to a struct containing all the ports and registers recieved from the ABAR
	Output:
		Type of device ( as an int )
*/
static int32 check_type(HBA_PORT *port)
{
	uint32 ssts = port->ssts;

	uint8 ipm = (ssts >> 8) & 0x0F;
	uint8 det = ssts & 0x0F;

	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
	{
		return AHCI_DEV_NULL;
	}
	if (ipm != HBA_PORT_IPM_ACTIVE)
	{
		return AHCI_DEV_NULL;
	}

	switch (port->sig)
	{
		case SATA_SIG_ATAPI:
		{
			return AHCI_DEV_SATAPI;
		}
		case SATA_SIG_SEMB:
		{
			return AHCI_DEV_SEMB;
		}
		case SATA_SIG_PM:
		{
			return AHCI_DEV_PM;
		}
		default:
		{
			return AHCI_DEV_SATA;
		}
	}
}
