#pragma once
#ifndef FIS_H
#define FIS_H

#include "types.h"
#include "screen.h"

typedef enum
{
	FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
	FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
	FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
	FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
	FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_TYPE;

typedef enum
{
	ATA_CMD_IDENTIFY     = 0xEC,
	ATA_CMD_READ_DMA     = 0xC8,
	ATA_CMD_READ_DMA_EX  = 0x25,
	ATA_CMD_WRITE_DMA    = 0xCA,
 	ATA_CMD_WRITE_DMA_EX = 0x35
} FIS_COMMAND;

typedef struct tagFIS_REG_H2D
{
	// DWORD 0
	uint8  fis_type;	// FIS_TYPE_REG_H2D

	uint8  pmport:4;	// Port multiplier
	uint8  rsv0:3;		// Reserved
	uint8  c:1;		// 1: Command, 0: Control

	uint8  command;	// Command register
	uint8  featurel;	// Feature register, 7:0

	// DWORD 1
	uint8  lba0;		// LBA low register, 7:0
	uint8  lba1;		// LBA mid register, 15:8
	uint8  lba2;		// LBA high register, 23:16
	uint8  device;		// Device register

	// DWORD 2
	uint8  lba3;		// LBA register, 31:24
	uint8  lba4;		// LBA register, 39:32
	uint8  lba5;		// LBA register, 47:40
	uint8  featureh;	// Feature register, 15:8

	// DWORD 3
	uint8  countl;		// Count register, 7:0
	uint8  counth;		// Count register, 15:8
	uint8  icc;		// Isochronous command completion
	uint8  control;	// Control register

	// DWORD 4
	uint8  rsv1[4];	// Reserved
} FIS_REG_H2D;


typedef struct tagFIS_REG_D2H
{
	// DWORD 0
	uint8  fis_type;    // FIS_TYPE_REG_D2H

	uint8  pmport:4;    // Port multiplier
	uint8  rsv0:2;      // Reserved
	uint8  i:1;         // Interrupt bit
	uint8  rsv1:1;      // Reserved

	uint8  status;      // Status register
	uint8  error;       // Error register

	// DWORD 1
	uint8  lba0;        // LBA low register, 7:0
	uint8  lba1;        // LBA mid register, 15:8
	uint8  lba2;        // LBA high register, 23:16
	uint8  device;      // Device register

	// DWORD 2
	uint8  lba3;        // LBA register, 31:24
	uint8  lba4;        // LBA register, 39:32
	uint8  lba5;        // LBA register, 47:40
	uint8  rsv2;        // Reserved

	// DWORD 3
	uint8  countl;      // Count register, 7:0
	uint8  counth;      // Count register, 15:8
	uint8  rsv3[2];     // Reserved

	// DWORD 4
	uint8  rsv4[4];     // Reserved
} FIS_REG_D2H;


typedef struct tagFIS_DATA
{
	// DWORD 0
	uint8  fis_type;	// FIS_TYPE_DATA

	uint8  pmport:4;	// Port multiplier
	uint8  rsv0:4;		// Reserved

	uint8  rsv1[2];	// Reserved

	// DWORD 1 ~ N
	uint32 data[1];	// Payload
} FIS_DATA;


typedef struct tagFIS_PIO_SETUP
{
	// DWORD 0
	uint8  fis_type;	// FIS_TYPE_PIO_SETUP

	uint8  pmport:4;	// Port multiplier
	uint8  rsv0:1;		// Reserved
	uint8  d:1;		// Data transfer direction, 1 - device to host
	uint8  i:1;		// Interrupt bit
	uint8  rsv1:1;

	uint8  status;		// Status register
	uint8  error;		// Error register

	// DWORD 1
	uint8  lba0;		// LBA low register, 7:0
	uint8  lba1;		// LBA mid register, 15:8
	uint8  lba2;		// LBA high register, 23:16
	uint8  device;		// Device register

	// DWORD 2
	uint8  lba3;		// LBA register, 31:24
	uint8  lba4;		// LBA register, 39:32
	uint8  lba5;		// LBA register, 47:40
	uint8  rsv2;		// Reserved

	// DWORD 3
	uint8  countl;		// Count register, 7:0
	uint8  counth;		// Count register, 15:8
	uint8  rsv3;		// Reserved
	uint8  e_status;	// New value of status register

	// DWORD 4
	uint16 tc;		// Transfer count
	uint8  rsv4[2];	// Reserved
} FIS_PIO_SETUP;


typedef struct tagFIS_DMA_SETUP
{
	// DWORD 0
	uint8  fis_type;	// FIS_TYPE_DMA_SETUP

	uint8  pmport:4;	// Port multiplier
	uint8  rsv0:1;		// Reserved
	uint8  d:1;		// Data transfer direction, 1 - device to host
	uint8  i:1;		// Interrupt bit
	uint8  a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed

    uint8  rsved[2];       // Reserved

	//DWORD 1&2

	uint64 DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

	//DWORD 3
	uint32 rsvd;           //More reserved

	//DWORD 4
	uint32 DMAbufOffset;   //Byte offset into buffer. First 2 bits must be 0

	//DWORD 5
	uint32 TransferCount;  //Number of bytes to transfer. Bit 0 must be 0

	//DWORD 6
	uint32 resvd;          //Reserved

} FIS_DMA_SETUP;


typedef struct tagFIS_DEV_BITS
{
	// DWORD 0
	uint8  fis_type;	// FIS_TYPE_DMA_SETUP (A1h)

	uint8  rsv0:5;		// Reserved
	uint8  r0:1;
	uint8  i:1;
	uint8  n:1;

	uint8 statusl:3;
	uint8 r1:1;
	uint8 statush:3;

	uint8 r2:1;
	uint8 error;

	// DWORD 1
	uint32 act;

} FIS_DEV_BITS;


/*
	Debug Functions:
*/
/*
void debug_FIS_REG_H2D(FIS_REG_H2D *ptr);
void debug_FIS_REG_D2H(volatile FIS_REG_D2H *ptr);
void debug_FIS_DATA(FIS_DATA *ptr);
void debug_FIS_PIO_SETUP(volatile FIS_PIO_SETUP *ptr);
void debug_FIS_DMA_SETUP(volatile FIS_DMA_SETUP *ptr);
*/
// void init_FIS_TYPE_REG_H2D(FIS_REG_H2D *fis);


#endif
