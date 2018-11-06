
#ifndef __RF_MAIN_H__
#define __RF_MAIN_H__

#include "typedef.h"

//#define _BUFFER_EN

#define MAX_BUFF	    1024//512//64//48
#define T_OUT_MAX 	    50 // MS
#define RF_TX_TOUT	    30 // MS

#define RF_IDLE_TOUT 	60*1000UL	 	// idle for sleep
#define RF_SEND_TOUT	1*1000UL		// tx data timeout
#define RF_RECV_TOUT	600//1*1000		// rx data timeout

#define INIT_MODE		0
#define DATA_MODE		1
#define CMD_MODE        2
#define INIT_ID			0x08//0x80

#define RING_SIZE       1024//256

#define TX_DAT_SIZE		54//56//57//29//45

typedef enum
{
	RECV_IDLE = 0,
	RECV_DATA,
	RECV_STOP,
	RECV_OVER
}RECV_STATUS;

typedef enum
{
	TX_OUT = 0,
	IDLE_OUT,
	INIT_OUT,
}TIME_OUT;

typedef enum
{
	ADD_NONE = 0,
	ADD_TAB,
	ADD_BLANK,
	ADD_LINE,
}ADD_TYPE;

typedef enum
{
	RF_OFFLINE = 0,
	RF_ONLINE,
	RF_CONFIG,
	RF_INITED,
	RF_PWR_OFF
}RF_STATE;

typedef struct
{
	RECV_STATUS state;
	uint8_t data_ok;
	uint8_t time_out;
	uint8_t ex_info;
	uint8_t cmd_type;
	uint8_t dat_ptr;
	UINT16 index;
	uint8_t index_tx;
	UINT16 index_tmp;
	UINT16 index_last;
	uint8_t buff[MAX_BUFF];
	uint8_t buff_tx[64];
	uint8_t buff_tmp[MAX_BUFF];
	//uint8_t buff_last[MAX_BUFF];
}UART_RECV;

typedef struct
{
	UINT16 tx_out;		// 1 tx data
	UINT32 idleout;		// sys idle
	UINT32 sleep_t;     // user set
	uint8_t tx_time;		// continue tx
	uint8_t status;		// rf state

	uint8_t store_flag;
	UINT32 start_addr;  // store+init
	UINT32 store_addr;	// store+init
	UINT32 cur_addr;	// init
	UINT32 up_addr;		// no init

	UINT32 total_num; 	// store+init
	UINT32 cur_num;		// reset
	UINT32 up_num;		// no init

	UINT16 dat_ptr;     // buf index
	uint8_t dat_buf[1100];// flash buf

	uint8_t beep;			// beep flag
	uint8_t pid;			// product id + store

	uint8_t same_flag;	// same data + store
	UINT16 same_out;	// same tine out
	UINT16 same_time;	// same time + store

	uint8_t rx_init;		// slave init
	uint8_t add_type;		// init + store
	uint8_t ticket;		// init + store

	uint8_t bt_mod;       // init + store
	
	uint8_t dat_mode;		// init + store
	uint8_t prefix;		// init + store
	uint8_t suffix1;		// init + store
	uint8_t suffix2;		// init + store
	uint8_t key_mode;		// init + store

	uint8_t pre_byte;
	uint8_t pst_byte;
	uint8_t add_ctrl;
}RF_HOST;

extern UART_RECV uart;
extern RF_HOST rf_host;
extern RING_CTRL uart_ring;
extern uint8_t uart_buffer[RING_SIZE];
extern uint8_t pid_mask;

void RF_Timer(void);
void BAR_UART_Rx(uint8_t dat);
void BAR_DataSave0(void);
BOOL RF_HostTout(TIME_OUT mode);
BOOL RF_HostTxDatas(uint8_t mode);
void Data_Adjustment1(uint8_t mode);
bool Wireless_Main(void);
bool Wireless_MainCmd(void);

bool RF_PairForRx(void);
uint32_t* GetMAC2_4G(void);

uint8_t getPkt_sid(void);


#endif /* __RF_MAIN_H__ */

