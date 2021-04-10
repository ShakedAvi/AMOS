#ifndef UHCI_H
#define UHCI_H

#define UHCI_COMMAND     0x00
#define UHCI_STATUS      0x02

#define UHCI_INTERRUPT   0x04
#define UHCI_FRAME_NUM   0x06
#define UHCI_FRAME_BASE  0x08
#define UHCI_SOF_MOD     0x0C

#define UHCI_PORT_WRITE_MASK  0x124E    //  0001 0010 0100 1110

#define UHCI_HUB_RESET_TIMEOUT  50

#define TOKEN_OUT    0xE1
#define TOKEN_IN     0x69
#define TOKEN_SETUP  0x2D

#define BREADTH (0<<2)
#define DEPTH   (1<<2)

#define QUEUE_HEAD_PTR_MASK  0xFFFFFFF0
#define QUEUE_HEAD_Q         0x00000002
#define QUEUE_HEAD_T         0x00000001

#define TD_PTR_MASK  0xFFFFFFF0
#define TD_VF        0x00000004
#define TD_Q         0x00000002
#define TD_T         0x00000001

#define TD_FLAGS_SPD      0x20000000
#define TD_FLAGS_CERR     0x18000000
#define TD_FLAGS_LS       0x04000000
#define TD_FLAGS_ISO      0x02000000
#define TD_FLAGS_IOC      0x01000000
#define TD_STATUS_ACTIVE  0x00800000
#define TD_STATUS_STALL   0x00400000
#define TD_STATUS_DBERR   0x00200000
#define TD_STATUS_BABBLE  0x00100000
#define TD_STATUS_NAK     0x00080000
#define TD_STATUS_CRC_TO  0x00040000
#define TD_STATUS_BSTUFF  0x00020000
#define TD_STATUS_MASK    0x00FF0000
#define TD_ACTLEN_MASK    0x000007FF

#define TD_INFO_MAXLEN_MASK   0xFFE00000
#define TD_INFO_MAXLEN_SHFT   21
#define TD_INFO_D             0x00080000
#define TD_INFO_ENDPT_MASK    0x00078000
#define TD_INFO_ENDPT_SHFT    15
#define TD_INFO_ADDR_MASK     0x00007F00
#define TD_INFO_ADDR_SHFT     8
#define TD_INFO_PID           0x000000FF

struct UHCI_TRANSFER_DESCRIPTOR {
  uint32   link_ptr;
  uint32   reply;
  uint32   info;
  uint32   buff_ptr;
  uint32   resv0[4];
};

struct UHCI_QUEUE_HEAD {
  uint32   horz_ptr;
  uint32   vert_ptr;
  uint32   resv0[2];
};

/*
  Resets the UHCI Controller ports, this is done in order to Detect it.
  Input:
    The start of the I/O bar memory address of the UHCI Controller
  Output:
    False if the reset failed, true otherwise
*/
int reset_uhci_controller(uint16 bar);

/*
  Setups the UHCI Controllers
  Input:
    The start of the I/O bar memory address of the UHCI Controller
  Output:
    False if the setup failed, true otherwise
*/
int setup_uhci_controller(uint16 bar);

/*
  Resets the Root Hub's port that the UHCI is attached to and enables the port if a device is attached to it
  Input:
    bar: The start of the I/O bar memory address of the UHCI Controller
    port: The offset to the Root Hub's port ( 0x10 )
  Output:
    False if the reset failed, true otherwise
*/
int reset_uhci_port(uint16 bar, uint8 port);

/*
  Checks if there is a valid port at the given Offset
  Input:
    bar: The start of the I/O bar memory address of the UHCI Controller
    port: The offset to the Root Hub's port ( 0x10 )
  Output:
    False if there is no port at the given offset, true otherwise
*/
int find_uhci_port(uint16 bar, uint8 port);

#endif
