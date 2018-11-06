#ifndef	__BSP_H__
#define	__BSP_H__

#include "MM32F103.h"


//中断回调函数类型
typedef void(* CallBackISR)(void);


#include "Common.h"
#include "BSPSystem.h"
#include "BSPWatchDog.h"
#include "BSPLED.h"
#include "BSPPower.h"
#include "BSPCodeScanner.h"
//#include "BSPBeep.h"
#include "BSPSound.h"
#include "BSPHID.h"
#include "BSPTimer.h"
#include "BSPKey.h"
#include "BSPFlash.h"
#include "BSPEEPROM.h"
#include "BSPWireless.h"



extern void BSPRoutine(void);


#endif



