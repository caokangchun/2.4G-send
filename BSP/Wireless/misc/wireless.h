
#ifndef __WIRELESS_H__
#define __WIRELESS_H__

/*----------- U S E R    M A C R O   D E F I N E -----------------------------*/
#define _RF_HOST
#define _BAR_UART
#define _BT_MODULE

/* system includes -----------------------------------------------------------*/
//#include <intrinsics.h>
#include <string.h>
#include "typedef.h"

#include "BSPTimer.h"
#include "checkout.h"
#include "ring_buffer.h"

/* user includes -------------------------------------------------------------*/
#include "rf_hal.h"
#include "rf_cmd.h"
#include "rf_main.h"
#include "bluetooth.h"

/* device includes ------------------------------------------------------------*/
//#include "PN3020.h"
//#include "PN3020_Def.h"
#include "xn297l.h"
#include "i435e.h"
#include "W25Qxx.h"


/* driver includes ------------------------------------------------------------*/
#include "utf8.h"
#include "cp936.h"

void delay_1ms(uint16_t n);
#endif /*end of __WIRELESS_H__ */

