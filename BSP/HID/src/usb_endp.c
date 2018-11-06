/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "MM32F103.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "BSPHID.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t PrevEp1XferComplete;
__IO uint8_t PrevEp2XferComplete;
extern void SetHidRxFlag(void);      //The funtion is in the BSPHID.c
extern uint8_t* GetHidRxBuff(void);		//The funtion is in ther BSPHID.c
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
    if ( dev_type == USBD_HID )
    {
        PrevEp1XferComplete = 1;
    }
    else if ( dev_type == USBD_VCP )
    {
        // to do:
    }
    else
    {
        while(1);
    }
}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_IN_Callback(void)
{
	if((_GetUSB_CTRL2() & EP2_CTRL_TRANEN)== 0)
	{
		PrevEp2XferComplete = 1;
		_SetUSB_CTRL2(0);
	}
}

void EP2_OUT_Callback(void)
{
	if(GetHidRxFlag() == 0)
	{
		PMAToUserBufferCopy(GetHidRxBuff(), ENDP2, CUSTOMHID_DATA_SIZE);
		SetHidRxFlag();
	}
}
/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
  //uint16_t USB_Rx_Cnt;
  
  /* Get the received data buffer and update the counter */
  //USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Buffer);
  
  /* USB data will be immediately processed, this allow next USB traffic being 
  NAKed till the end of the USART Xfer */
  
  //USB_To_USART_Send_Data(USB_Rx_Buffer, USB_Rx_Cnt);
 
  /* Enable the receive of data on EP3 */
  //SetEPRxValid(ENDP3);
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

