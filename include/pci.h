#ifndef PCI_H
#define PCI_H

#include "system.h"
#include "util.h"
#include "string.h"
#include "screen.h"
#include "ahci.h"
#include "paging.h"

typedef union pci_dev
{
    uint32 bits;
    struct {
        uint32 always_zero    : 2;
        uint32 field_num      : 6;
        uint32 function_num   : 3;
        uint32 device_num     : 5;
        uint32 bus_num        : 8;
        uint32 reserved       : 7;
        uint32 enable         : 1;
    };
} pci_dev_t;

/*typedef struct devices
{
  uint32 device_id;
  uint32 vendor_id;
  uint8 class_id;
  uint8 subclass_id;
}devices*/;

// Ports
#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

// Config Address Register

// Offset
#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0a
#define PCI_CLASS                0x0b
#define PCI_CACHE_LINE_SIZE      0x0c
#define PCI_LATENCY_TIMER        0x0d
#define PCI_HEADER_TYPE          0x0e
#define PCI_BIST                 0x0f
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

// Device type
#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106
#define PCI_NONE 0xFFFF

//Address of BAR5, Used to get the data we need from the AHCI driver.
#define HBA_MEM_BASE_ADDRESS 0xfebd5000
#define DEVICE_PER_BUS           32
#define FUNCTION_PER_DEVICE      32

/*
	Reads from PCI via port 0xCFC (PCI_CONFIG_DATA)
	Input: PCI Device ( Saved as the Union pci_dev_t, contains data about the device like bus number, device number and function number)
				 Field: Offset of data to read (Vendor Id, Class Id, Device Id ect.)
  Output:
				 Data from PCI
*/
uint32 pci_read(pci_dev_t dev, uint32 field);

/*
    Writes into a PCI Field
		Input:
			PCI Device ( Saved as the Union pci_dev_t, contains data about the device like bus number, device number and function number)
			Field: Offset of data to read (Vendor Id, Class Id, Device Id ect.)
			Value to write into the PCI Field
		Output:
			None
*/
void pci_write(pci_dev_t dev, uint32 field, uint32 value);

/*
    Get device type (i.e, is it a bridge, ide controller ? mouse controller? etc)
		Input:
			PCI Device ( Saved as the Union pci_dev_t, contains data about the device like bus number, device number and function number)
		Output:
			The Device Type.
*/
uint32 get_device_type(pci_dev_t dev);

/*
		Get secondary bus from a PCI bridge device
		Input:
			PCI Device ( Saved as the Union pci_dev_t, contains data about the device like bus number, device number and function number)
		Output:
			The Secondary Bus.
*/
uint32 get_secondary_bus(pci_dev_t dev);

/*
    Checks if current device is end point.
		Input:
			PCI Device ( Saved as the Union pci_dev_t, contains data about the device like bus number, device number and function number)
		Output:
			1 if we reached the end, zero otherwise.
*/
uint32 pci_reach_end(pci_dev_t dev);

/*
	The following three functions are doing recursion, enumerating each and every device connected to pci
	We start with the primary bus 0, which has 8 function, each of the function is actually a bus
	Then, each bus can have 8 devices connected to it, each device can have 8 functions
	When scanning the function, we can check which device we've found based on it's vendor id and class id.
*/

// Scan function
pci_dev_t pci_scan_function(uint32 bus, uint32 device, uint32 function, uint8 class_id, uint8 subclass_id, uint8 prog_if);

// Scan device
pci_dev_t pci_scan_device(uint32 bus, uint32 device, uint8 class_id, uint8 subclass_id, uint8 prog_if);

// Scan bus
pci_dev_t pci_scan_bus(uint32 bus, uint8 class_id, uint8 subclass_id, uint8 prog_if);

/*
    Initializes the PCI, filling size for each field in config space
		Input: None
		Output: None
*/
void pci_init();

/*
	Returns the data from the ABAR (AHCI Base Memory Register), the abar contains all AHCI registers and memories.
	Input:
		None
	Output:
		HBA_MEM* abar: a struct containing the data from the ABAR
*/
HBA_MEM* get_abar();

/*
	Searches for USB Host Controllers in the PCI
	Input: None
	Output: None
*/
uint16 find_usb_host_controllers();

#endif
