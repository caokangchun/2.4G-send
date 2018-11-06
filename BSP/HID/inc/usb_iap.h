
#ifndef _USB_APP_H_
#define _USB_APP_H_

#include <stdint.h>
#include <string.h>
#include "BSPHID.h"

#define _HID_CMD_LEN        256
#define _CMD_STX_LEN        8

#define _PAGE_SIZE          1024
#define _DT_MAX_LEN         128

#define _CMD_PTR            8

#define _BOOT_ADDR          56*1024UL

/* usb SETUP struct */
typedef struct _usb_setup
{
	uint8_t bmRequestType;
	uint8_t bRequest;
  	uint8_t wValue_l;
  	uint8_t wValue_h;
  	uint8_t wIndex_l;
  	uint8_t wIndex_h;
  	uint8_t wLength_l;
  	uint8_t wLength_h;
}usb_setup;

typedef struct _usb_report_s
{
    uint8_t rpt_flag;
	uint8_t dat_total;
	uint8_t dat_index;
	uint8_t *dat_buff;//[_HID_CMD_LEN];
	uint16_t time_out;
}usb_report_s;

typedef struct _user_flash_s
{
    uint32_t wr_ptr;
    uint16_t pkt_no;
	uint8_t rd_buff[_DT_MAX_LEN];
}user_flash_s;

typedef enum _usb_hid_cmd
{
    HALT_CMD    = 0x06,
	SET_SNR 	= 0x82,
	GET_SNR 	= 0x83,
	WR_USRINFO  = 0x84,
    WR_CARD     = 0x21,
	READ_CARD   = 0x25,
	AUTO_READ 	= 0x0F,
	SET_BUZZER 	= 0x89,
	SET_LED 	= 0x88,
    // IAP Command
    IAP_JUMP_S  = 0xA0,
    IAP_STAT_S  = 0xA1,
    IAP_DATA_S  = 0xA2,
    IAP_FAIL_S  = 0xA3,
    IAP_EXIT_S  = 0xA4,
    IAP_JUMP_H  = 0xB0,
    IAP_STAT_H  = 0xB1,
    IAP_DATA_H  = 0xB2,
    IAP_FAIL_H  = 0xB3,
    IAP_EXIT_H  = 0xB4,
}usb_hid_cmd;

typedef enum _usb_hid_resp
{
	STATUS_OK 	= 0x00,
	STATUS_ERR 	= 0x01,	
	SET_OK		= 0x80,
	SET_ERR		= 0x81,
	NOT_EXIST 	= 0x83,
	PARA_ERR  	= 0x89,
	CMD_ERR   	= 0x8F
}usb_hid_resp;

extern bool system_reboot;
extern uint32_t iap_timeout;
extern usb_report_s usb_report;
extern user_flash_s user_flash;

void usb_report_in(uint8_t *report);

#endif /* _USB_APP_H_ */

