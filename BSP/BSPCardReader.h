#ifndef	__CARDREADER_H__
#define	__CARDREADER_H__


#include "Common.h"
#include "BSP.h"


extern bool BSPCardReaderInit(void);

extern bool BSPCardReaderAllowRead(void);

extern uint8_t BSPCardReaderRead(uint8_t *Data);


#endif



