#ifndef	__RFID_H__
#define	__RFID_H__

#include <stdlib.h>
#include "Common.h"


typedef struct
{
	uint8_t Data[8];
}
UID_Data;


extern void RFIDInit(void);

extern bool RFIDAllowRead(void);

extern UID_Data RFIDRead(void);

extern void RFIDRoutine(void);


#endif
