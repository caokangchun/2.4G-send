
#ifndef _BT_FUNC_H_
#define _BT_FUNC_H_

#include "typedef.h"

#define BT_OUT_MAX 	    30 // MS
#define BT_BUFF         64

typedef enum
{
	BT_IDLE,
	BT_DATA,
	BT_STOP,
	BT_OVER
}BT_STATUS;

typedef struct
{
	volatile BT_STATUS state;
	volatile UINT16 rx_tout;
	volatile uint8_t data_ok;
	uint8_t init;
	uint8_t linked;
	uint8_t time_out;
	uint8_t index_tmp;
	uint8_t buff_tmp[BT_BUFF];
	uint8_t resp_len;
	uint8_t resp_buf[BT_BUFF];
}BT_CTRL;

extern BT_CTRL bt;

void BT_Timer(void);
void BT_UART_Rx(uint8_t dat);
BOOL BT_PairClear(void);
BOOL BT_Reconnect(void);
BOOL BT_ConnectReq(void);
void BT_Disconnect(void);
BOOL BT_Initial(void);
BOOL BT_TxTPDatas(uint8_t *pdata, UINT16 len);
BOOL BT_TxHidDatas(uint8_t *pdata, uint8_t len, uint8_t type);



#endif /* _BT_FUNC_H_ */

