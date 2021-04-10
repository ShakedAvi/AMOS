
#ifndef USB_H
#define USB_H

//#pragma pack(1)

// Controller Types
#define XHC_TYPE_UHCI  0x00
#define XHC_TYPE_OHCI  0x10
#define XHC_TYPE_EHCI  0x20
#define XHC_TYPE_XHCI  0x30

enum { CONTROL_EP=0, ISOCHRONOUS_EP, BULK_EP, INTERRUPT_EP };  // transfer types (Endpoint types) (USB 2.0 page 270)

// Reset wait times.  USB 2.0 specs, page 153, section 7.1.7.5, paragraph 3
#define USB_TDRSTR   50   // reset on a root hub
#define USB_TDRST    10   // minimum delay for a reset
#define USB_TRHRSI    3   // No more than this between resets for root hubs
#define USB_TRSTRCY  10   // reset recovery

struct DEVICE_DESC {
  uint8  len;
  uint8  type;
  uint16 usb_ver;
  uint8  _class;
  uint8  subclass;
  uint8  protocol;
  uint8  max_packet_size;
  uint16 vendorid;
  uint16 productid;
  uint16 device_rel;
  uint8  manuf_indx;   // index value
  uint8  prod_indx;    // index value
  uint8  serial_indx;  // index value
  uint8  configs;      // Number of configurations
};

struct REQUEST_PACKET {
  uint8  request_type;
  uint8  request;
  uint16 value;
  uint16 index;
  uint16 length;
};

// config descriptor
struct CONFIG_DESC {
  uint8  len;
  uint8  type;
  uint16 tot_len;
  uint8  num_interfaces;
  uint8  config_val;
  uint8  config_indx;
  uint8  bm_attrbs;
  uint8  max_power;
};

struct STRING_DESC {
  uint8  len;         // length of whole desc in bytes
  uint8  type;
  uint16 string[127];
};

struct INTERFACE_ASSOSIATION_DESC {
  uint8  len;             // len of this desc (8)
  uint8  type;            // type = 11
  uint8  interface_num;   // first interface number to start association
  uint8  count;           // count of continuous interfaces for association
  uint8  _class;          //
  uint8  subclass;        //
  uint8  protocol;        //
  uint8  function_indx;   // string id of this association
};

// interface descriptor
struct INTERFACE_DESC {
  uint8  len;
  uint8  type;
  uint8  interface_num;
  uint8  alt_setting;
  uint8  num_endpoints;
  uint8  interface_class;
  uint8  interface_sub_class;
  uint8  interface_protocol;
  uint8  interface_indx;
};

// endpoint descriptor
struct ENDPOINT_DESC {
  uint8  len;
  uint8  type;
  uint8  end_point;        // 6:0 end_point number, 7 = IN (set) or OUT (clear)
  uint8  bm_attrbs;        //
  uint16 max_packet_size;  // 10:0 = max_size, 12:11 = max transactions, 15:13 = reserved
  uint8  interval;
};

// setup packets
#define DEV_TO_HOST     0x80
#define HOST_TO_DEV     0x00
#define REQ_TYPE_STNDRD 0x00
#define REQ_TYPE_CLASS  0x20
#define REQ_TYPE_VENDOR 0x40
#define REQ_TYPE_RESV   0x60
#define RECPT_DEVICE    0x00
#define RECPT_INTERFACE 0x01
#define RECPT_ENDPOINT  0x02
#define RECPT_OTHER     0x03
#define STDRD_GET_REQUEST   (DEV_TO_HOST | REQ_TYPE_STNDRD | RECPT_DEVICE)
#define STDRD_SET_REQUEST   (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_DEVICE)
#define STDRD_SET_INTERFACE (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_INTERFACE)

// device requests
enum { GET_STATUS=0, CLEAR_FEATURE, SET_FEATURE=3, SET_ADDRESS=5, GET_DESCRIPTOR=6, SET_DESCRIPTOR,
        GET_CONFIGURATION, SET_CONFIGURATION,
// interface requests
        GET_INTERFACE, SET_INTERFACE,
// standard endpoint requests
        SYNCH_FRAME,
// Device specific
        GET_MAX_LUNS = 0xFE, BULK_ONLY_RESET
};

// Descriptor types
enum {
  DEVICE=1,
  CONFIG,
  STRING,
  INTERFACE,
  ENDPOINT,
  DEVICE_QUALIFIER,
  OTHER_SPEED_CONFIG,
  INTERFACE_POWER,
  OTG,
  DEBUG,
  INTERFACE_ASSOSIATION,

  HID=0x21,
  HID_REPORT,
  HID_PHYSICAL,

  INTERFACE_FUNCTION = 0x24,
  ENDPOINT_FUNCTION,

  HUB=0x29
};

#endif
