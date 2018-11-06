#ifndef __I435E_H__
#define __I435E_H__		1


#include "MM32F103.h"


#include "UART2.h"



typedef enum 
{
	READY = 0,
	CONNECT,
	DISCONNECT,
	OK,
	UNKNOW,
}BTState;


extern void I435eUartInit(void);

extern uint8_t I435eRead(void);

extern void ResponseParse(uint8_t ResponseLen);

extern void I435eWrite(char* dat,uint8_t len);

extern void I435eSendContent(char* content,uint8_t len);

extern BTState GetBTStatus(void);


#endif


