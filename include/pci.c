#include "pci.h"

uint32 pci_size_map[100];
pci_dev_t dev_zero = {0};

/*
	The function reads from PCI via port 0xCFC (PCI_CONFIG_DATA)
	Input:
		PCI Device ( Saved as the Union pci_dev_t, contains data about the device like bus number, device number and function number)
		Field: Offset of data to read (Vendor Id, Class Id, Device Id ect.)
  Output:
		Data from PCI
*/
uint32 pci_read(pci_dev_t dev, uint32 field) {
	// Only most significant 6 bits of the field
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	outportl(PCI_CONFIG_ADDRESS, dev.bits);

	// What size is this field supposed to be ?
	uint32 size = pci_size_map[field];
	if(size == 1)
  {
		// Get the first byte only, since it's in little endian, it's actually the 3rd byte
		uint8 t = inportb(PCI_CONFIG_DATA + (field & 3));
		return t;
	}
	else if(size == 2)
  {
		uint16 t = inports(PCI_CONFIG_DATA + (field & 2));
		return t;
	}
	else if(size == 4)
  {
		// Read entire 4 bytes
		uint32 t = inportl(PCI_CONFIG_DATA);
		return t;
	}
	return 0xffff;
}

/*
  The function writes pci field.

  Input:
    The address of the frame to set up.
  Output:
    None.
*/
void pci_write(pci_dev_t dev, uint32 field, uint32 value)
{
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	// Tell where we want to write
	outportl(PCI_CONFIG_ADDRESS, dev.bits);
	// Value to write
	outportl(PCI_CONFIG_DATA, value);
}

/*
    Get device type (i.e, is it a bridge, ide controller ? mouse controller? etc)
*/
uint32 get_device_type(pci_dev_t dev)
{
	uint32 t = pci_read(dev, PCI_CLASS) << 8;
	return t | pci_read(dev, PCI_SUBCLASS);
}

/*
		Get secondary bus from a PCI bridge device
*/
uint32 get_secondary_bus(pci_dev_t dev)
{
	return pci_read(dev, PCI_SECONDARY_BUS);
}

/*
    Is current device an end point ? PCI_HEADER_TYPE 0 is end point
*/
uint32 pci_reach_end(pci_dev_t dev)
{
	uint32 t = pci_read(dev, PCI_HEADER_TYPE);
	return !t;
}

/*
	The following three functions are basically doing recursion, enumerating each and every device connected to pci
	We start with the primary bus 0, which has 8 function, each of the function is actually a bus
	Then, each bus can have 8 devices connected to it, each device can have 8 functions
	When we gets to enumerate the function, check if the vendor id and device id match, if it does, we've found our device !
*/

/*
    Scan function
*/
pci_dev_t pci_scan_function(uint32 bus, uint32 device, uint32 function)
{
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;
	dev.function_num = function;

	// If it's a PCI Bridge device, get the bus it's connected to and keep searching
	if(get_device_type(dev) == PCI_TYPE_BRIDGE)
  {
		pci_scan_bus(get_secondary_bus(dev));
	}

	uint32 device_id  = pci_read(dev, PCI_DEVICE_ID);
	uint32 vendor_id = pci_read(dev, PCI_VENDOR_ID);
	uint8 class_id = pci_read(dev, PCI_CLASS);
	uint8 subclass_id = get_device_type(dev);

	if(class_id == 0x01 && subclass_id == 0x06 && pci_read(dev, PCI_PROG_IF) == 0x01)
	{
		print("AHCI FOUND! \n");
		return dev;
	}
	return dev_zero;
}

/*
    Scan device
*/
pci_dev_t pci_scan_device(uint32 bus, uint32 device)
{
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;

	if(pci_read(dev, PCI_VENDOR_ID) == PCI_NONE)
	{
		return dev_zero;
	}

	pci_dev_t t = pci_scan_function(bus, device, 0);
	if(t.bits)
	{
		return t;
	}

	if(pci_reach_end(dev))
	{
		return dev_zero;
	}

	for(int function = 1; function < FUNCTION_PER_DEVICE; function++)
  {
		if(pci_read(dev,PCI_VENDOR_ID) != PCI_NONE)
    {
			t = pci_scan_function(bus, device, function);
			if(t.bits)
			{
				return t;
			}
		}
	}
	return dev_zero;
}

/*
    Scan bus
*/
pci_dev_t pci_scan_bus(uint32 bus)
{
	for(int device = 0; device < DEVICE_PER_BUS; device++)
  {
		pci_dev_t t = pci_scan_device(bus, device);
		if(t.bits)
		{
			return t;
		}
	}
	return dev_zero;
}

/*
	Returns the data from the ABAR (AHCI Base Memory Register), the abar contains all AHCI registers and memories.
	Input:
		None
	Output:
		HBA_MEM* abar: a struct containing the data from the ABAR
*/
HBA_MEM* get_abar()
{
	pci_dev_t ahci_driver = pci_scan_bus(0);
	print("Reading ABAR...\n");
	uint32 bar5 = pci_read(ahci_driver, PCI_BAR5) & 0xFFFFFFF0;
	/*if(bar5 == HBA_MEM_BASE_ADDRESS)
	{
		print("Epic");
	}*/
	uint32 base_addr = HBA_MEM_BASE_ADDRESS;
	//HBA_MEM* abar = (HBA_MEM*)(base_addr + (1024*768*4));
	virtual_map_pages(base_addr - 0x1000, 0x10000000, 1, 1);
	HBA_MEM* abar = (HBA_MEM*)base_addr;
	return abar;
}

/*
    PCI Init, filling size for each field in config space
		Input: None
		Output: None
*/
void pci_init()
{
	// Init size map
	pci_size_map[PCI_VENDOR_ID] =	2;
	pci_size_map[PCI_DEVICE_ID] =	2;
	pci_size_map[PCI_COMMAND]	=	2;
	pci_size_map[PCI_STATUS]	=	2;
	pci_size_map[PCI_SUBCLASS]	=	1;
	pci_size_map[PCI_CLASS]		=	1;
	pci_size_map[PCI_CACHE_LINE_SIZE]	= 1;
	pci_size_map[PCI_LATENCY_TIMER]		= 1;
	pci_size_map[PCI_HEADER_TYPE] = 1;
	pci_size_map[PCI_BIST] = 1;
	pci_size_map[PCI_BAR0] = 4;
	pci_size_map[PCI_BAR1] = 4;
	pci_size_map[PCI_BAR2] = 4;
	pci_size_map[PCI_BAR3] = 4;
	pci_size_map[PCI_BAR4] = 4;
	pci_size_map[PCI_BAR5] = 4;
	pci_size_map[PCI_INTERRUPT_LINE]	= 1;
	pci_size_map[PCI_SECONDARY_BUS]		= 1;

}
