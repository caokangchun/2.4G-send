#ifndef	__CM3800B_H__
#define	__CM3800B_H__


#include "BSP.h"


extern void CM3800BInit(void);
extern void CM3800BOpen(void);
extern void CM3800BClose(void);
extern void CM3800BScanStart(void);
extern void CM3800BScanStop(void);
extern bool CM3800BAllowRead(void);
extern uint8_t CM3800BRead(void);
extern void CM3800BRoutine(void);


#endif

