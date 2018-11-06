
#include "includes.h"
#include "ConfigAddr.h"

const uint8_t rf_ack[ACK_R][ACK_C] =
{
	STX, 0x01, 0xFE, ETX, 
	STX, 0x02, 0xFD, ETX, 
	STX, 0x03, 0xFC, ETX, 
	STX, 0x04, 0xFB, ETX, 
};

//code uint8_t rf_ping[4] = {STX, 0x02, 0xFD, ETX};
//code uint8_t rf_data[4] = {STX, 0x04, 0xFB, ETX};
//code uint8_t rf_idle[4] = {STX, 0x08, 0xF8, ETX};

const uint8_t rf_ch_list[PIPE_INDEX_MAX+1][CH_INDEX_MAX] = 
{
    0, 25,50,
    1, 26,51,
    2, 27,52,
    3, 28,53,

    4, 29,54, 
    5, 30,55, 
    6, 31,56, 
    7, 32,57,

    8, 33,58,
    9, 34,59,
    10,35,60,
    11,36,61,

    12,37,62, 
    13,38,63, 
    14,39,64,
    15,40,65,

    16,41,66,
    17,42,67,
    18,43,68,
    19,44,69,

    20,45,70,
    21,46,71,
    22,47,72,
    23,48,73,
    24,49,74,

	80,80,80, // Sync channel
};



void delay_1ms(uint16_t n)
{
	while(n--)
	{
		BSPTimerDelay(500);	//us
		BSPTimerDelay(500);	//us
	}
}

//---------------------------------------------------------------------------------------
//	Description	: 	rf change to next channel
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void rf_goto_next_ch(void)
{
	xn297l.ch_index++;

	/* get next channel index */
	if(xn297l.ch_index == CH_INDEX_MAX)
	{
		xn297l.ch_index = 0;
	}
}

//---------------------------------------------------------------------------------------
//	Description	: 	rf change to next pipe
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
void rf_set_pipe(uint8_t pipe_x)
{
	xn297l.pipe_index = pipe_x;// & 0x0F;
}

//---------------------------------------------------------------------------------------
//	Description	: 	rf channel no
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void rf_get_ch_no(void)
{
	xn297l.ch_no = rf_ch_list[xn297l.pipe_index][xn297l.ch_index];
}


//---------------------------------------------------------------------------------------
//	Description	: 	rf channel no
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
uint8_t rf_set_ch(void)
{
	uint8_t ch_no;
	
	/* rf goto next channel */
	rf_goto_next_ch();

	/* rf get ch no */
	rf_get_ch_no();

	/* set rf channel */
	xn297l_set_channel(xn297l.ch_no);

	/* read out current rf ch no */
	xn297l_read_1byte(REG_RF_CH | CMD_R_REGISTER, &ch_no);

	return ch_no;
}



//---------------------------------------------------------------------------------------
//	Description	: 	rf set mac
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2016 - 06 - 02
//---------------------------------------------------------------------------------------
void rf_set_mac(uint8_t *mac)
{
	xn297l_set_mac(mac);
}

//---------------------------------------------------------------------------------------
//	Description	: 	rf init
//	Parameters 	: 	none
//	Return 		: 	true/false - init success/fail
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
BOOL rf_init(void)
{
	uint8_t i;
	uint32_t mac=0;
	
    sRF_Configuration();
	
	BSPEEPROMReadBytes(CONFIG_2_4MAC, (uint16_t*)(&mac), 2);
	mac=0xac6d3987;
	memcpy(xn297l.mac, &mac, 4);

    rf_set_pipe(((uint8_t)(mac>>24) + 1) % PIPE_INDEX_MAX); // MAC -> RF_PIPE

	/* try init rf for 3 times */
	for(i = 0; i < 3; i++)
	{
		if(xn297l_init())
		{	
            rf_set_ch();
			xn297l_goto_sleep();
			rf_host.status = RF_ONLINE;
			return TRUE;
		}

		delay_1ms(10);
	}

	return FALSE;
}

//---------------------------------------------------------------------------------------
//	Description	: 	rf send data
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
BOOL rf_transceiver (
	RF_DIR rf_dir, uint8_t t_out,
	uint8_t *tx_src, uint8_t tx_len,
	uint8_t *rx_dst, uint8_t dst_len, uint8_t *rx_len
)
{
	BOOL flag = FALSE;

	/* rf wakeup first */
	xn297l_goto_wakeup();

	while(1)
	{
		BSPWatchDogReset();
		
		flag = xn297l_transceive(rf_dir, 0, t_out, \
								 tx_src, tx_len, \
								 rx_dst, dst_len, rx_len);
		//printf("*");
		
	#ifdef _RF_HOST
		/* rx ack ok */
		if(flag)
		{
			break;
		}

		/* rf send timeout */
		if(RF_HostTout(TX_OUT))
		{
			break;
		}
	#else // _RF_SLAVE
		/* wait untill receive data */
		if(flag)
		{
			break;
		}
		
		/* rf recv timeout */
		if(rf_rx_tout())
		{
			break;
		}
	#endif
		
		rf_set_ch();
//		rf_set_ch111();
	}
	
	/* rf goto sleep */
	xn297l_goto_sleep();

	return flag;
}

