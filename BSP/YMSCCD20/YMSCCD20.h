#ifndef	__YMSCCD20_H__
#define	__YMSCCD20_H__


#include "BSP.h"


extern void YMSCCD20Init(void);
extern void YMSCCD20Open(void);
extern void YMSCCD20ScanStart(void);
extern void YMSCCD20ScanStop(void);
extern bool YMSCCD20AllowRead(void);
extern uint8_t YMSCCD20Read(void);
extern void YMSCCD20Routine(void);


#endif

