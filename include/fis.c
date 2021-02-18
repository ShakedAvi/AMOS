/*#include "fis.h"
#include "ahci.h"

void debug_FIS_REG_H2D(FIS_REG_H2D *ptr) {
	print("---debug_FIS_REG_H2D---\n");
	print("fis_type %x\n", ptr->fis_type);
	print("c %x\n", ptr->c);
	print("command %x\n", ptr->command);
	print("lba0 %x\n", ptr->lba0);
	print("lba1 %x\n", ptr->lba1);
	print("lba2 %x\n", ptr->lba2);
	print("device %x\n", ptr->device);
	print("lba3 %x\n", ptr->lba3);
	print("lba4 %x\n", ptr->lba4);
	print("lba5 %x\n", ptr->lba5);
	print("countl %x counth %x\n", ptr->countl, ptr->counth);
	print("------\n");
}

void debug_FIS_REG_D2H(volatile FIS_REG_D2H *ptr) {
	print("---debug_FIS_REG_D2H---\n");
	print("fis_type %x\n", ptr->fis_type);
	print("i %x\n", ptr->i);
	print("status %x\n", ptr->status);
	print("lba0 %x\n", ptr->lba0);
	print("lba1 %x\n", ptr->lba1);
	print("lba2 %x\n", ptr->lba2);
	print("device %x\n", ptr->device);
	print("lba3 %x\n", ptr->lba3);
	print("lba4 %x\n", ptr->lba4);
	print("lba5 %x\n", ptr->lba5);
	print("countl %x counth %x\n", ptr->countl, ptr->counth);
	print("------\n");
}

void debug_FIS_DATA(FIS_DATA *ptr) {
	print("---debug_FIS_DATA---\n");
	print("fis_type %x\n", ptr->fis_type);
	print("data %x\n", ptr->data[0]);
	print("------\n");
}

void debug_FIS_PIO_SETUP(volatile FIS_PIO_SETUP *ptr) {
	print("---FIS_PIO_SETUP---\n");
	print("fis_type %x\n", ptr->fis_type);
	print("d %x i %x\n", ptr->d, ptr->i);
	print("status %x\n", ptr->status);
	print("lba0 %x\n", ptr->lba0);
	print("lba1 %x\n", ptr->lba1);
	print("lba2 %x\n", ptr->lba2);
	print("device %x\n", ptr->device);
	print("lba3 %x\n", ptr->lba3);
	print("lba4 %x\n", ptr->lba4);
	print("lba5 %x\n", ptr->lba5);
	print("countl %x counth %x\n", ptr->countl, ptr->counth);
	print("------\n");
}

void debug_FIS_DMA_SETUP(volatile FIS_DMA_SETUP *ptr) {
	print("---FIS_DMA_SETUP---\n");
	// TODO
	print("------\n");
}
*/
