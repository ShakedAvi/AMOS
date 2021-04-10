#include "task.h"
#include "uhci.h"
#include "usb.h"
#include "system.h"

/*
  Resets the UHCI Controller ports, this is done in order to Detect it.
  Input:
    The start of the I/O bar memory address of the UHCI Controller
  Output:
    False if the reset failed, true otherwise
*/
int reset_uhci_controller(uint16 bar)
{
  print("Reseting The UHCI Controller...");
  for (int i = 0; i < 5; i++)
  {
    outportw(bar+UHCI_COMMAND, 0x0004);
    sleep();
    outportw(bar+UHCI_COMMAND, 0x0000);
  }

  // is the command register its default value of 0x0000 ?
  if (inportw(bar+UHCI_COMMAND) != 0x0000) return 0;
  // is the status register its default value of 0x0020 ?
  if (inportw(bar+UHCI_STATUS) != 0x0020) return 0;
  // The status register is write clear, let's clear it out.
  outportw(bar+UHCI_STATUS, 0x00FF);

  if (inportb(bar+UHCI_SOF_MOD) != 0x40) return 0;

  outportw(bar+UHCI_COMMAND, 0x0002);
  sleep();
  if (inportw(bar+UHCI_COMMAND) & 0x0002) return 0;
  return 1;
}

/*
  Setups the UHCI Controllers
  Input:
    The start of the I/O bar memory address of the UHCI Controller
  Output:
    False if the setup failed, true otherwise
*/
int setup_uhci_controller(uint16 bar)
{
  outportw(bar + UHCI_INTERRUPT, 0x000F);
  outportw(bar + UHCI_FRAME_NUM, 0x0000);
  outportl(bar + UHCI_FRAME_BASE, 0x12345000); //Testing
  outportb(bar + UHCI_SOF_MOD, 0x40);
  outportw(bar + UHCI_STATUS, 0x001F);
  outportw(bar + UHCI_COMMAND, (1<<7) | (1<<6) | (1<<0));
}

/*
  Resets the Root Hub's port that the UHCI is attached to and enables the port if a device is attached to it
  Input:
    bar: The start of the I/O bar memory address of the UHCI Controller
    port: The offset to the Root Hub's port ( 0x10 )
  Output:
    False if the reset failed, true otherwise
*/
int reset_uhci_port(uint16 bar, uint8 port)
{
  uint16 value_from_port;
  uint16 flag = 0;
  outportw(bar + port, inportw(bar + port) | (1<<9));
  sleep();
  outportw(bar + port, inportw(bar + port) & ~(1<<9));
  for (int i = 0; i < 10; i++)
  {
    sleep();
    value_from_port = inportw(bar + port);

    // if bit 0 is clear, nothing is attached thus we do not enable the port.
    if(!(value_from_port & (1<<0)))
    {
      print("\nNo Device\n");
      flag = 1;
      break;
    }

    if (value_from_port & ((1<<3) | (1<<1)))
    {
      outportw(bar + port, value_from_port & UHCI_PORT_WRITE_MASK);
      continue;
    }

    // if enable bit is set, break.
    if (value_from_port & (1<<2))
    {
      flag = 1;
      break;
    }

    print("Enabling Set Bit...\n");
    // else, set the anable bit.
    outportw(bar + port, value_from_port | (1<<2));
  }

  return flag;
}

/*
  Checks if there is a valid port at the given Offset
  Input:
    bar: The start of the I/O bar memory address of the UHCI Controller
    port: The offset to the Root Hub's port ( 0x10 )
  Output:
    False if there is no port at the given offset, true otherwise
*/
int find_uhci_port(uint16 bar, uint8 port)
{
  // if bit 7 is 0, not a port
 if ((inportw(bar + port) & 0x0080) == 0) return 0;

 // Clearing the port
 outportw(bar + port, inportw(bar + port) & ~0x0080);
 if ((inportw(bar + port) & 0x0080) == 0) return 0;

 // WC the port
 outportw(bar + port, inportw(bar + port) | 0x0080);
 if ((inportw(bar + port) & 0x0080) == 0) return 0;

 // write a 1 to bits 3:1, if they come back as zero this is a port
 outportw(bar + port, inportw(bar + port) | 0x000A);
 if ((inportw(bar + port) & 0x000A) != 0) return 0;

 return 1;
}
