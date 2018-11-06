/**
  ******************************************************************************
  * @file    Project/MT031x_StdPeriph_Templates/main.h 
  * @author  XXXXXXXX Software Team
  * @version V1.0.0
  * @date    11/01/2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, XXXXXXXX SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 XXXXXXXX</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
    
//#define _UID_CHK // UID Crypto Check Enable

/* Exported functions ------------------------------------------------------- */
extern void Initial_Mcu(void);
extern void BarCode(void);
extern void KeyProc(void);
extern void IniSet(void);
extern void INT_PCA(void);
extern void INT_INT0(void);
extern void INT_INT1(void);
extern void INT_TIMER0(void);
extern void ReceiveWave(void);
extern void USB_Initial(uint8_t usb_type);
extern void usb_iap_jump(void);

//#include "usb_conf.h"
//#include "usb_core.h"
//#include "usb_pwr.h"
//#include "hw_config.h"

extern uint8_t	dev_type;
extern const char *SW_VERSION;

#ifdef __cplusplus
}
#endif

#endif /*__MAIN_H */

/******************* (C) COPYRIGHT XXXXXXXX *****END OF FILE****/

