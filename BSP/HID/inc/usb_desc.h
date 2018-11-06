/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Descriptor Header for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "BSPHID.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define CUSTOMHID_SIZ_HID_DESC                  0x09

#define CUSTOMHID_OFF_HID_C_DESC                43
#define CUSTOMHID_OFF_HID_K_DESC                18

#define CUSTOMHID_SIZ_DEVICE_DESC               18
#define CUSTOMHID_SIZ_CONFIG_DESC               66	// Custom HID + Keyboard
#define CUSTOMHID_SIZ_REPORT_C_DESC             33  // Custom HID
#define CUSTOMHID_SIZ_REPORT_K_DESC             65  // Keyboard
#define CUSTOMHID_SIZ_STRING_LANGID             4
#define CUSTOMHID_SIZ_STRING_VENDOR             32
#define CUSTOMHID_SIZ_STRING_PRODUCT            38
#define CUSTOMHID_SIZ_STRING_SERIAL             34
#define VIRTUAL_COM_PORT_DATA_SIZE              64
#define VIRTUAL_COM_PORT_INT_SIZE               8

#define VIRTUAL_COM_PORT_SIZ_DEVICE_DESC        18
#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        67
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID      4
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR      32
#define VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT     42
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL      34

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/* Exported functions ------------------------------------------------------- */
extern const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC];
extern uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC];
extern const uint8_t CustomHID_ReportCDescriptor[CUSTOMHID_SIZ_REPORT_C_DESC];
extern const uint8_t CustomHID_ReportKDescriptor[CUSTOMHID_SIZ_REPORT_K_DESC];
extern const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID];
extern const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR];
extern const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT];
extern uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL];

/* Exported functions ------------------------------------------------------- */
extern const uint8_t Virtual_Com_Port_DeviceDescriptor[VIRTUAL_COM_PORT_SIZ_DEVICE_DESC];
extern const uint8_t Virtual_Com_Port_ConfigDescriptor[VIRTUAL_COM_PORT_SIZ_CONFIG_DESC];

extern const uint8_t Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID];
extern const uint8_t Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR];
extern const uint8_t Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT];
extern uint8_t Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL];
extern void SetCustomHIDMaxPower(uint8_t power);
extern void SetCustomHIDMaxInterval(uint8_t num);
#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
