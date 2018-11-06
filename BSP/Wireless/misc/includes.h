
#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/* user macro defines --------------------------------------------------------*/

/* system includes -----------------------------------------------------------*/
#include <string.h>
#include "BSP.h"
#include "typedef.h"

/* user includes -------------------------------------------------------------*/
#include "main.h"
#include "MM32x103_it.h"

/* device includes -----------------------------------------------------------*/

/* driver includes -----------------------------------------------------------*/
#include "drv_gpio.h"
#include "drv_exti.h"
#include "drv_timer.h"
#include "drv_flash.h"
#include "drv_uart.h"
//#include "drv_watchdog.h"

//#include "usbapp.h"
//#include "gpio_func.h"
#include "wireless.h"






void delay_1ms(uint16_t n);

#endif /*end of __INCLUDES_H__ */

