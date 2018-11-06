
#include "usb_iap.h"
#include "BSPHID.h"
//bool iap_flag = 0;
usb_report_s usb_report = {0};

extern void SystemIap(void);

/*************************************************************************
*	Function	: 	usb_report_init
*	Description : 	init report data 
*	Parameters 	: 	data_length -> report valid data length
*	Return 		: 	None
*	Author 		: 	Sycreader
*	Date 		: 	2013 - 09 - 17
***************************************************************************/
static void usb_report_init(uint8_t data_length)
{
	/* clear usb report buff data first */
	memset(usb_report.dat_buff, 0 , _HID_CMD_LEN);
	
	usb_report.dat_buff[0] = 0x03;	// report id + 7 bytes 0
	// between all be zero
	usb_report.dat_buff[8] = 0xAA;//STX;	// data head
	usb_report.dat_buff[9] = 0; 	// machine id
	usb_report.dat_buff[10] = data_length;//data length	
	//user data proc...
	usb_report.dat_buff[data_length + 12] = 0xBB;//ETX; // data end
}

/*************************************************************************
*	Function	: 	usb_return_status
*	Description : 	return err report 
*	Parameters 	: 	err_type -> error type
*	Return 		: 	None
*	Author 		: 	Sycreader
*	Date 		: 	2013 - 10 - 28
***************************************************************************/
static void usb_ret_status(uint8_t status, uint8_t ret_type)
{
	usb_report_init(2); // 2 bytes data

	usb_report.dat_buff[11] = status; 	// STATUS
	usb_report.dat_buff[12] = ret_type;	// ok
	
	/* XOR check for valid data (except STX+ETX) */
	usb_report.dat_buff[13] = usb_report.dat_buff[10] ^ \
							  usb_report.dat_buff[11] ^ \
							  usb_report.dat_buff[12];
    //usb_ret_datas();
    //memcpy(ret_buf, usb_report.dat_buff, _HID_CMD_LEN);
}

/*************************************************************************
*	Function	: 	usb_get_report
*	Description : 	usb get report
*	Parameters 	: 	None
*	Return 		: 	None
*	Author 		: 	Sycreader
*	Date 		: 	2013 - 09 - 17
***************************************************************************/
void usb_report_in(uint8_t *rpt_buf)
{
    usb_report.dat_buff = rpt_buf;

    switch(usb_report.dat_buff[_CMD_PTR + 3])
    {
        case IAP_JUMP_S : // iap jump main
        {
          //  iap_flag = 1;
            usb_ret_status(STATUS_OK, SET_OK);
            break;
        }
   
	    default :
    	{
    	    usb_ret_status(STATUS_ERR, CMD_ERR); // error cmd
    	    break;
    	}
	}
}

void SystemIap(void)
{
//    #define BOOT_ADDR 0x8009C00
//    typedef  void (*jumpfun)(void);
//    jumpfun jump2FE;

//   // Watchdog_feed();
//    NVIC->ICER[0] = 0xFFFFFFFF;
//    jump2FE=(jumpfun)*(volatile uint32_t*)(BOOT_ADDR+4);
//    __set_MSP(*(volatile uint32_t*)BOOT_ADDR);
//    jump2FE();
//    while(1);
}

/*************************************************************************
*	Function	: 	usb_get_report
*	Description : 	usb get report
*	Parameters 	: 	None
*	Return 		: 	None
*	Author 		: 	Sycreader
*	Date 		: 	2013 - 09 - 17
***************************************************************************/
void usb_iap_jump(void)
{    
//    if(iap_flag)
//    {
//        iap_flag = 0;
//        delay_1ms(10);
//        USB_Cable_Config(DISABLE);
//        SystemIap();
//    }
}

