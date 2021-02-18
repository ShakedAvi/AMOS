#ifndef AHCI_H
#define AHCI_H

#include "util.h"

#define	AHCI_BASE	0x400000	// 4M

#define HBA_PXIS_TFES (1 << 30)

#define HBA_PXCMD_ST 0x0001
#define HBA_PXCMD_FRE 0x0010
#define HBA_PXCMD_FR 0x4000
#define HBA_PXCMD_CR 0x8000

#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	0x96690101	// Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

#include "fis.h"

typedef volatile struct tagHBA_PORT
{
	uint32 clb;		// 0x00, command list base address, 1K-byte aligned
	uint32 clbu;		// 0x04, command list base address upper 32 bits
	uint32 fb;		// 0x08, FIS base address, 256-byte aligned
	uint32 fbu;		// 0x0C, FIS base address upper 32 bits
	uint32 is;		// 0x10, interrupt status
	uint32 ie;		// 0x14, interrupt enable
	uint32 cmd;		// 0x18, command and status
	uint32 rsv0;		// 0x1C, Reserved
	uint32 tfd;		// 0x20, task file data
	uint32 sig;		// 0x24, signature
	uint32 ssts;		// 0x28, SATA status (SCR0:SStatus)
	uint32 sctl;		// 0x2C, SATA control (SCR2:SControl)
	uint32 serr;		// 0x30, SATA error (SCR1:SError)
	uint32 sact;		// 0x34, SATA active (SCR3:SActive)
	uint32 ci;		// 0x38, command issue
	uint32 sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	uint32 fbs;		// 0x40, FIS-based switch control
	uint32 rsv1[11];	// 0x44 ~ 0x6F, Reserved
	uint32 vendor[4];	// 0x70 ~ 0x7F, vendor specific
} HBA_PORT;

typedef volatile struct tagHBA_MEM
{
	// 0x00 - 0x2B, Generic Host Control
	uint32 cap;		// 0x00, Host capability
	uint32 ghc;		// 0x04, Global host control
	uint32 is;		// 0x08, Interrupt status
	uint32 pi;		// 0x0C, Port implemented
	uint32 vs;		// 0x10, Version
	uint32 ccc_ctl;	// 0x14, Command completion coalescing control
	uint32 ccc_pts;	// 0x18, Command completion coalescing ports
	uint32 em_loc;		// 0x1C, Enclosure management location
	uint32 em_ctl;		// 0x20, Enclosure management control
	uint32 cap2;		// 0x24, Host capabilities extended
	uint32 bohc;		// 0x28, BIOS/OS handoff control and status

	// 0x2C - 0x9F, Reserved
	uint8  rsv[0xA0-0x2C];

	// 0xA0 - 0xFF, Vendor specific registers
	uint8  vendor[0x100-0xA0];

	// 0x100 - 0x10FF, Port control registers
	HBA_PORT	ports[1];	// 1 ~ 32
} HBA_MEM;

typedef volatile struct tagHBA_FIS
{
	// 0x00
	FIS_DMA_SETUP	dsfis;		// DMA Setup FIS
	uint8         pad0[4];

	// 0x20
	FIS_PIO_SETUP	psfis;		// PIO Setup FIS
	uint8         pad1[12];

	// 0x40
	FIS_REG_D2H	rfis;		// Register – Device to Host FIS
	uint8         pad2[4];

	// 0x58
	FIS_DEV_BITS	sdbfis;		// Set Device Bit FIS

	// 0x60
	uint8         ufis[64];

	// 0xA0
	uint8   	rsv[0x100-0xA0];
} HBA_FIS;

typedef struct tagHBA_CMD_HEADER
{
	// DW0
	uint8  cfl:5;		// Command FIS length in DWORDS, 2 ~ 16
	uint8  a:1;		// ATAPI
	uint8  w:1;		// Write, 1: H2D, 0: D2H
	uint8  p:1;		// Prefetchable

	uint8  r:1;		// Reset
	uint8  b:1;		// BIST
	uint8  c:1;		// Clear busy upon R_OK
	uint8  rsv0:1;		// Reserved
	uint8  pmp:4;		// Port multiplier port

	uint16 prdtl;		// Physical region descriptor table length in entries

	// DW1
	volatile
	uint32 prdbc;		// Physical region descriptor byte count transferred

	// DW2, 3
	uint32 ctba;		// Command table descriptor base address
	uint32 ctbau;		// Command table descriptor base address upper 32 bits

	// DW4 - 7
	uint32 rsv1[4];	// Reserved
} HBA_CMD_HEADER;

typedef struct tagHBA_PRDT_ENTRY
{
	uint32 dba;		// Data base address
	uint32 dbau;		// Data base address upper 32 bits
	uint32 rsv0;		// Reserved

	// DW3
	uint32 dbc:22;		// Byte count, 4M max
	uint32 rsv1:9;		// Reserved
	uint32 i:1;		// Interrupt on completion
} HBA_PRDT_ENTRY;

typedef struct tagHBA_CMD_TBL
{
	// 0x00
	uint8  cfis[64];	// Command FIS

	// 0x40
	uint8  acmd[16];	// ATAPI command, 12 or 16 bytes

	// 0x50
	uint8  rsv[48];	// Reserved

	// 0x80
	HBA_PRDT_ENTRY	prdt_entry[1];	// Physical region descriptor table entries, 0 ~ 65535
} HBA_CMD_TBL;

void port_rebase(HBA_PORT *port, int portno);
void start_cmd(HBA_PORT *port);
void stop_cmd(HBA_PORT *port);
HBA_PORT* probe_port(HBA_MEM *abar);
static int check_type(HBA_PORT *port);
#endif
