/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Descriptors for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0xFF,                       /*idVendor (0x0483)*/
    0xFF,
    0x35,                       /*idProduct = 0x5750*/
    0x00,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* CustomHID_DeviceDescriptor */
void SetCustomHIDMaxPower(uint8_t power)
{
	CustomHID_ConfigDescriptor[8] = power;
}
void SetCustomHIDMaxInterval(uint8_t num)
{
	CustomHID_ConfigDescriptor[33] = num;
}
/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuation Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x02,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: Configuration value */
    0x00,         /* iConfiguration: Index of string descriptor describing
                                 the configuration*/
    0xA0,         /* bmAttributes: Bus powered */
    0XFA,         /* MaxPower 500 mA: this current is used for detecting Vbus */

    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x01,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x01,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x11,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    CUSTOMHID_SIZ_REPORT_K_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    0x81,          /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    0x08,          /* wMaxPacketSize: 8 Bytes max */
    0x00,
    0x01,          /* bInterval: Polling Interval (32 ms) */
    /************** Descriptor of Custom HID interface ****************/
    /* 34 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x01,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x02,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID ********************/
    /* 43 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    CUSTOMHID_SIZ_REPORT_C_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
	/******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */
    0x82,          /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    CUSTOMHID_DATA_SIZE,          /* wMaxPacketSize: 8 Bytes max */
    0x00,
    0x02,          /* bInterval: Polling Interval (32 ms) */
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */
    0x02,          /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    CUSTOMHID_DATA_SIZE,          /* wMaxPacketSize: 8 Bytes max */
    0x00,
    0x02,          /* bInterval: Polling Interval (32 ms) */
    /* 52 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportKDescriptor[CUSTOMHID_SIZ_REPORT_K_DESC] =
  {    
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    
	//0x85, 0x01, 				    // REPORT_ID (1)

	// modifier key
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)

	// reserved byte
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

	// normal key
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)

	// leds
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)

	// padding bits
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    
    0xc0,							// END_COLLECTION
  }; /* CustomHID_ReportDescriptor */
const uint8_t CustomHID_ReportCDescriptor[CUSTOMHID_SIZ_REPORT_C_DESC] =
  {                    
		0x05, 0x8c, /* USAGE_PAGE (ST Page) */
        0x09, 0x01, /* USAGE (Demo Kit) */
        0xa1, 0x01, /* COLLECTION (Application) */
       // 0x85, 0x01,                    //   REPORT_ID (1)
        0x09,0x03, // USAGE ID - Vendor defined
        0x15,0x00, // LOGICAL_MINIMUM (0)
        0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255)
        0x75,0x08, // REPORT_SIZE (8bit)
        0x95,CUSTOMHID_DATA_SIZE, // REPORT_COUNT (64Byte)
        0x81,0x02, // INPUT (Data,Var,Abs)
        // The Output report
        0x09,0x04, // USAGE ID - Vendor defined
        0x15,0x00, // LOGICAL_MINIMUM (0)
        0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255)
        0x75,0x08, // REPORT_SIZE (8bit)
        0x95,CUSTOMHID_DATA_SIZE, // REPORT_COUNT (64Byte)
        0x91,0x02, // OUTPUT (Data,Var,Abs)
        0xc0 /* END_COLLECTION */
  }; /* CustomHID_ReportC_Descriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'U', 0, 'M', 0, 'C', 0, 'O', 0, 'D', 0, 'E', 0, ' ', 0,
    'S', 0, 'C', 0, 'A', 0, 'N', 0, 'N', 0, 'E', 0, 'R', 0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'K', 0, 'e', 0, 'y', 0, 'b', 0, 'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0,
    'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0,
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'U', 0, 'M', 0,'C', 0, 'O' , 'D', 0 ,'E',
  };

/* USB Standard Device Descriptor */
const uint8_t Virtual_Com_Port_DeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,
    0x02,   /* bcdUSB = 2.00 */
    0x02,   /* bDeviceClass: CDC */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 */
    0x4A,
    0x29,   /* idVendor = 0x0483 */
    0x00,
    0x01,   /* idProduct = 0x7540 */
    0x00,
    0x02,   /* bcdDevice = 2.00 */
    1,              /* Index of string descriptor describing manufacturer */
    2,              /* Index of string descriptor describing product */
    3,              /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

const uint8_t Virtual_Com_Port_ConfigDescriptor[] =
  {
    /*Configuration Descriptor*/
    0x09,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
    0x00,
    0x02,   /* bNumInterfaces: 2 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xA0,   /* bmAttributes: bus powered */
    0xFA,   /* MaxPower 500 mA */
    /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */
    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,
    /*Call Management Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    0x01,   /* bDataInterface: 1 */
    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */
    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */
    /*Endpoint 1 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x81,   /* bEndpointAddress: (IN1) */
    0x03,   /* bmAttributes: Interrupt */
    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */
    0x00,
    0xFF,   /* bInterval: */
    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    0x01,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */
    /*Endpoint 3 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x03,   /* bEndpointAddress: (OUT3) */
    0x02,   /* bmAttributes: Bulk */
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */
    /*Endpoint 2 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x82,   /* bEndpointAddress: (IN2) */
    0x02,   /* bmAttributes: Bulk */
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
    0x00,
    0x00    /* bInterval */
  };

/* USB String Descriptors */
const uint8_t Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const uint8_t Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'U', 0, 'M', 0, 'C', 0, 'O', 0, 'D', 0, 'E', 0, ' ', 0,
    'S', 0, 'C', 0, 'A', 0, 'N', 0, 'N', 0, 'E', 0, 'R', 0
  };

const uint8_t Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "STM32 Virtual COM Port" */
    'U', 0, 'S', 0, 'B', 0, ' ', 0,
    'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0,
    'C', 0, 'O', 0, 'M', 0, ' ', 0,
    'P', 0, 'o', 0, 'r', 0, 't', 0,
  };

uint8_t Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'S', 0, 'U', 0, 'M', 0, 'C', 0, 'O', 0 ,'D', 0 ,'E'
  };

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

