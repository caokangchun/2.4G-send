
#include "includes.h"

const uint8_t rf_def_para[5] = {MAC0, MAC1, MAC2, MAC3, 0x00};

RF_CONTROL xn297l =
{
	/*.ch_no     = */0,
	/*.ch_index  = */0,
	/*.ch_pipe 	 = */0,
	/*.mac[0] 	 = */MAC0,
	/*.mac[1] 	 = */MAC1,
	/*.mac[2] 	 = */MAC2,
	/*.mac[3] 	 = */MAC3,
	/*.power     = */0,
};

const uint8_t xn297l_init_seq[] = 
{
#if 0
    ( CMD_RST_FSPI /*0x53*/ ), 0x01, ( 0x5A ), /* Software Reset */
    ( CMD_RST_FSPI /*0x53*/ ), 0x01, ( 0xA5 ),
#endif /* 0 */
    
    ( CMD_ACTIVATE /*0x50*/ ), 0x01, ( 0x73 ), /* R_RX_PL_WID Enable */
	( CMD_FLUSH_RX /*0xE2*/ ), 0x01, ( 0x00 ), /* CLEAR RXFIFO */
    ( CMD_FLUSH_TX /*0xE1*/ ), 0x01, ( 0x00 ), /* CLEAR TXFIFO */
    
    ( REG_STATUS    /*0x07*/ | CMD_W_REGISTER ), 0x01, ( _RX_DR | _TX_DS | _MAX_RT ),
    ( REG_EN_RXADDR /*0x02*/ | CMD_W_REGISTER ), 0x01, ( _ENRX_P0 ),
    ( REG_SETUP_AW  /*0x03*/ | CMD_W_REGISTER ), 0x01, ( _SETUP_AW_4B ), /* 4byte width */
    ( REG_RF_CH     /*0x05*/ | CMD_W_REGISTER ), 0x01, ( 0x08 ), /* default ch8 */
    ( REG_RX_PW_P0  /*0x11*/ | CMD_W_REGISTER ), 0x01, ( RF_FIFO_MAX ), /* set 32 bytes */
    ( REG_TX_ADDR   /*0x10*/ | CMD_W_REGISTER ), 0x04, MAC0, MAC1, MAC2, MAC3,
    ( REG_RX_ADDR_P0/*0x0A*/ | CMD_W_REGISTER ), 0x04, MAC0, MAC1, MAC2, MAC3,
#ifdef RF_250KBPS
	( REG_BB_CAL	/*0x1F*/ | CMD_W_REGISTER ), 0x05, 0x12, 0xEC, 0x6F, 0xA1, 0x46,
	( REG_RF_CAL2	/*0x1A*/ | CMD_W_REGISTER ), 0x06, 0xD5, 0x21, 0xEB, 0x2C,0x5A, 0x40,
	( REG_DEMOD_CAL /*0x19*/ | CMD_W_REGISTER ), 0x01, ( 0x1F ),
	( REG_RF_CAL	/*0x1E*/ | CMD_W_REGISTER ), 0x03, 0xF6, 0x37, 0x5D,
	( REG_DEM_CAL2	/*0x1B*/ | CMD_W_REGISTER ), 0x03, 0x0B, 0xDF, 0x02,
#else
    ( REG_BB_CAL    /*0x1F*/ | CMD_W_REGISTER ), 0x05, /*0x0A*/0x3F, 0x6D, 0x67, 0x9C, 0x46,
    ( REG_RF_CAL2   /*0x1A*/ | CMD_W_REGISTER ), 0x06, 0x45, 0x21, 0xEF, 0x2C, 0x5A, 0x50,
    ( REG_DEMOD_CAL /*0x19*/ | CMD_W_REGISTER ), 0x01, ( 0x01 ),
    ( REG_RF_CAL    /*0x1E*/ | CMD_W_REGISTER ), 0x03, 0xF6, 0x37, 0x5D,
    ( REG_DEM_CAL2  /*0x1B*/ | CMD_W_REGISTER ), 0x03, 0x0B, 0xDF, 0x02,
#endif
    ( REG_DYNPD     /*0x1C*/ | CMD_W_REGISTER ), 0x01, ( _DPL_P0 ), /* Pipe0 Dynamic data length */
#ifdef RF_CE_PKT_SOFT
    ( REG_FEATURE   /*0x1D*/ | CMD_W_REGISTER ), 0x01, ( _CE_SEL | _EN_DPL | _EN_ACK_PAY ), 
#else
    ( REG_FEATURE   /*0x1D*/ | CMD_W_REGISTER ), 0x01, ( _EN_DPL | _EN_ACK_PAY ), 
#endif /* RF_CE_PKT_SOFT */
    ( REG_RF_SETUP  /*0x06*/ | CMD_W_REGISTER ), 0x01, ( _RF_RATE | _RF_POWER ), /* Power and Tx Rate*/
#if 1
    ( REG_SETUP_RETR/*0x04*/ | CMD_W_REGISTER ), 0x01, ( _ARD_1000us | _ARC_2 ),
    ( REG_EN_AA     /*0x01*/ | CMD_W_REGISTER ), 0x01, ( _ENAA_P0 ), /* _ENAA_P0 */
#else
    ( REG_SETUP_RETR/*0x04*/ | CMD_W_REGISTER ), 0x01, ( 0x00 ),
    ( REG_EN_AA     /*0x01*/ | CMD_W_REGISTER ), 0x01, ( 0x00 ),
#endif /* 0 */

	0xFF // reg int end sign
};


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l reg write
//	Parameters 	: 	reg_addr -> reg addr, reg_dat -> data to be write
//	Return 		: 	None
//	Author 		: 	RAY
//	Date 		: 	2013 - 03 - 26
//---------------------------------------------------------------------------------------
static void xn297l_write_1byte(uint8_t reg_addr, uint8_t reg_dat)
{	
	XN297L_CS_0;
	
	//xn297l_spi_mode(SPI_LOW_FALLING_TX);
	xn297l_spi_send(reg_addr); //reg address
	xn297l_spi_send(reg_dat);  //reg data

	XN297L_CS_1;
	//xn297l_spi_reset();
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l reg write
//	Parameters 	: 	reg_addr -> reg addr, reg_dat -> data to be write
//	Return 		: 	None
//	Author 		: 	RAY
//	Date 		: 	2013 - 03 - 26
//---------------------------------------------------------------------------------------
static void xn297l_write_bytes(uint8_t reg_addr, uint8_t *reg_dat, uint8_t dat_len)
{
	uint8_t i;
	
	XN297L_CS_0;
	
	//xn297l_spi_mode(SPI_LOW_FALLING_TX);
	xn297l_spi_send(reg_addr); //reg address

	for(i = 0; i < dat_len; i++)
	{
		xn297l_spi_send(*reg_dat++); //reg data
	}

	XN297L_CS_1;
	//xn297l_spi_reset();
}

#if 0
static void xn297l_write_bytes1(uint8_t reg_addr, const uint8_t *reg_dat, uint8_t dat_len)
{
	uint8_t i;
	
	XN297L_CS_0;
	
	//xn297l_spi_mode(SPI_LOW_FALLING_TX);
	xn297l_spi_send(reg_addr); //reg address

	for(i = 0; i < dat_len; i++)
	{
		xn297l_spi_send(*reg_dat++); //reg data
	}

	XN297L_CS_1;
	//xn297l_spi_reset();
}
#endif /* 0 */

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l reg read
//	Parameters 	: 	reg_addr -> reg address
//	Return 		: 	read out data
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
void xn297l_read_1byte(uint8_t reg_addr, uint8_t *reg_dat)
{	
	XN297L_CS_0;
	
	//xn297l_spi_mode(SPI_LOW_FALLING_TX);
	xn297l_spi_send(reg_addr);//reg address

	//xn297l_spi_mode(SPI_LOW_RISING_RX);
	*reg_dat = xn297l_spi_recv(0); //reg data
	
	XN297L_CS_1;
	//xn297l_spi_reset();
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l reg read
//	Parameters 	: 	reg_addr -> reg address
//	Return 		: 	read out data
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
static void xn297l_read_bytes(uint8_t reg_addr, uint8_t *reg_dat, uint8_t dat_len)
{
	uint8_t i;
	
	XN297L_CS_0;
	
	//xn297l_spi_mode(SPI_LOW_FALLING_TX);
	xn297l_spi_send(reg_addr);//reg address

	//xn297l_spi_mode(SPI_LOW_RISING_RX);

	for(i = 0; i < dat_len; i++)
	{
		*reg_dat++ = xn297l_spi_recv(0); //reg data
	}
	
	XN297L_CS_1;
	//xn297l_spi_reset();
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l sleep mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
void xn297l_clear_tx_fifo(void)
{
	xn297l_write_1byte(CMD_FLUSH_TX, 0);
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l sleep mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
void xn297l_clear_rx_fifo(void)
{
	xn297l_write_1byte(CMD_FLUSH_RX, 0);
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l write fifo
//	Parameters 	: 	src - data buff pointer, length - data length
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 04 - 17
//---------------------------------------------------------------------------------------
static void xn297l_write_pld_fifo(uint8_t *src, uint8_t length)
{
	if(length > RF_FIFO_MAX) /* fifo max = 32 */
	{
		length = RF_FIFO_MAX;
	}
	
	xn297l_write_bytes(CMD_W_TX_PAYLOAD, src, length);
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l write fifo
//	Parameters 	: 	src - data buff pointer, length - data length
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 04 - 17
//---------------------------------------------------------------------------------------
static void xn297l_write_ack_fifo(uint8_t pipex, uint8_t *src, uint8_t length)
{
	if(length > RF_FIFO_MAX) /* fifo max = 32 */
	{
		length = RF_FIFO_MAX;
	}
	
	xn297l_write_bytes(CMD_W_ACK_PAYLOAD | pipex, src, length);
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l read fifo
//	Parameters 	: 	dst - data buff pointer, rx_len - data length
//	Return 		: 	true/false - get data success/fail
//	Author 		: 	RAY
//	Date		:	2013 - 04 - 17
//---------------------------------------------------------------------------------------
static void xn297l_fifo_read(uint8_t *dst, uint8_t dst_len, uint8_t *rx_len)
{
	uint8_t fifo_len;

	xn297l_read_1byte(CMD_R_RX_PL_WID, &fifo_len);
	//printf("fifo len=%d\r\n", (UINT16)fifo_len);
	//uart_send_byte(fifo_len);

	if(fifo_len != 0)
	{
    	if(dst_len > fifo_len)
    	{
    		dst_len = fifo_len;
    	}
    	
    	xn297l_read_bytes(CMD_R_RX_PAYLOAD, dst, dst_len);
	}
	else
	{
	    dst_len = 0;
	}
	
	*rx_len = dst_len;
}




//---------------------------------------------------------------------------------------
//	Description	: 	xn297l mac config
//	Parameters 	: 	mac - sync word
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
void xn297l_set_mac(uint8_t *mac)
{
	xn297l_write_bytes(REG_TX_ADDR | CMD_W_REGISTER , mac, 4);
	xn297l_write_bytes(REG_RX_ADDR_P0 | CMD_W_REGISTER , mac, 4);
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l rf channel config
//	Parameters 	: 	rf_ch - rf channel
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
void xn297l_set_channel(uint8_t rf_ch)
{
	xn297l_write_1byte(REG_RF_CH | CMD_W_REGISTER, rf_ch & (~ BIT_MASK(7)));
}

#ifdef RF_CE_PKT_SOFT

//static INT8 ce_status = -1;

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l rf channel config
//	Parameters 	: 	rf_ch - rf channel
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
void xn297l_set_ce_1(void)
{
    //if(ce_status != 1)
    {
        //ce_status = 1;
	    xn297l_write_1byte(CMD_CE_FSPI_ON, 0);
	}
}

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l rf channel config
//	Parameters 	: 	rf_ch - rf channel
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
void xn297l_set_ce_0(void)
{
    //if(ce_status != 0)
    {
        //ce_status = 0;
	    xn297l_write_1byte(CMD_CE_FSPI_OFF, 0);
	}
}

#endif /* RF_CE_PKT_SOFT */

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l idle mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 04 - 15
//---------------------------------------------------------------------------------------
void xn297l_goto_stb1(void)
{
    RF_CE_SET_LOW;

	xn297l_write_1byte(REG_CONFIG | CMD_W_REGISTER, \
						( _PWR_UP | _MASK_RX_DR | _MASK_TX_DS | _MASK_MAX_RT | _EN_CRC | _CRC_2B |
					#ifdef _RF_HOST
						0 ));
					#else
						_PRIM_RX));
					#endif
}

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l idle mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 04 - 15
//---------------------------------------------------------------------------------------
void xn297l_goto_stb3(void)
{
    RF_CE_SET_LOW;

	xn297l_write_1byte(REG_CONFIG | CMD_W_REGISTER, \
						( _EN_PM | _PWR_UP | _MASK_RX_DR | _MASK_TX_DS | _MASK_MAX_RT | _EN_CRC | _CRC_2B |
					#ifdef _RF_HOST
						0 ));
					#else
						_PRIM_RX));
					#endif
}

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l sleep mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
void xn297l_goto_sleep(void)
{
    RF_CE_SET_LOW;

	xn297l_write_1byte(REG_CONFIG | CMD_W_REGISTER, \
						( _MASK_RX_DR | _MASK_TX_DS | _MASK_MAX_RT | _EN_CRC | _CRC_2B |
					#ifdef _RF_HOST
						0 ));
					#else
						_PRIM_RX));
					#endif
}

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l idle mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 04 - 15
//---------------------------------------------------------------------------------------
void xn297l_goto_wakeup(void)
{
    xn297l_goto_stb1(); // standby 1 state
    
    delay_1ms(2);  // wait startup done
}

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l sleep mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
void xn297l_goto_pd(void)
{
    uint8_t dat[6] = {0xd5, 0x21, 0xef, 0x2c, 0x5a, 0x42};
    
    RF_CE_SET_LOW;

	xn297l_write_bytes(REG_RF_CAL2 | CMD_W_REGISTER, dat, 6);
}

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void xn297l_goto_tx(void)
{
    RF_CE_SET_LOW;

	xn297l_write_1byte(REG_CONFIG | CMD_W_REGISTER, \
						( _EN_PM | _MASK_RX_DR | _EN_CRC | _CRC_2B | _PWR_UP ));

	xn297l_clear_tx_fifo();

#if 1
    // delay 380us
	BSPTimerDelay(250);//10);
	BSPTimerDelay(130);
#else
    BSPTimerDelay(10);
#endif /* 0 */
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void xn297l_goto_rx(void)
{
    RF_CE_SET_LOW;

	xn297l_write_1byte(REG_CONFIG | CMD_W_REGISTER, \
						( _EN_PM | _MASK_TX_DS | _MASK_MAX_RT | _EN_CRC | _CRC_2B | _PWR_UP | _PRIM_RX ));
						
	xn297l_clear_rx_fifo();
	
#ifdef RF_250KBPS
    delay_1ms(2);
#else
    // delay 320us
	BSPTimerDelay(250);
	BSPTimerDelay(70);
#endif /* 0 */
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void xn297l_tx_start(void)
{
#ifdef RF_250KBPS
	RF_CE_SET_HIGH;
	delay_1ms(2);
#else
    RF_CE_SET_HIGH;
    
	BSPTimerDelay(25); // at least 20us
	
    RF_CE_SET_LOW;
#endif /* RF_250KBPS */
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void xn297l_rx_start(void)
{
    RF_CE_SET_HIGH;

#ifdef RF_250KBPS
    delay_1ms(2);
#endif /* RF_250KBPS */
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void xn297l_clear_irq_flag(uint8_t irq_flag)
{
	xn297l_write_1byte(REG_STATUS | CMD_W_REGISTER, irq_flag & (_RX_DR | _TX_DS | _MAX_RT));
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static uint8_t xn297l_get_irq_flag(void)
{
	uint8_t flag;
	
	xn297l_read_1byte(REG_STATUS | CMD_R_REGISTER, &flag);

	return (flag & ( _RX_DR | _TX_DS | _MAX_RT ));
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l tx mode
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
static void xn297l_chip_reset(void)
{
    RF_CE_SET_LOW;

	xn297l_write_1byte(CMD_RST_FSPI, 0x5A);
	delay_1ms(5);
	xn297l_write_1byte(CMD_RST_FSPI, 0xA5);
	//xn297l_write_1byte(CMD_CE_FSPI_OFF, 0x00);
	delay_1ms(20);
}


//---------------------------------------------------------------------------------------
//	Description	: 	xn297l initial
//	Parameters 	: 	none
//	Return 		: 	true/false - init success/fail
//	Author 		: 	RAY
//	Date		:	2013 - 03 - 26
//---------------------------------------------------------------------------------------
BOOL xn297l_init(void)
{
	uint8_t addr;
	uint8_t bytes;
	const uint8_t *dat = xn297l_init_seq;//NULL;
	uint8_t index = 0;

	//dat = xn297l_init_seq;
	
    xn297l_chip_reset();

	while(1)
	{
		addr = dat[index++];
		
		if(addr != 0xFF)
		{
			bytes = dat[index++];
			//printf("byte:%d\r\n", (UINT16)bytes);
			
			if(bytes != 0x01)
			{
				//xn297l_write_bytes1(addr, &dat[index], bytes);
				xn297l_write_bytes(addr, (uint8_t *)&dat[index], bytes);
			}
			else
			{
				uint8_t tmp;
				
				xn297l_write_1byte(addr, dat[index]);

				tmp = addr & ~CMD_W_REGISTER;
				/* check bank0 0~6 reg */
				if((tmp < 0x07) || (0x11 == tmp) || (0x1C == tmp) || (0x1D == tmp))
				{
					xn297l_read_1byte(tmp, &tmp);
					//printf("wr=%x,rd=%x\r\n", (UINT16)(dat[index]), (UINT16)tmp);

					/* check if write success */
					if(tmp != (dat[index]))
					{
						//printf("xn297l init err.\r\n");
						return FALSE;
					}
				}
			}
			
			index += bytes;
		}
		else
		{
			break;
		}
	}
	
	delay_1ms(1);
	
	xn297l_set_channel(xn297l.ch_no);
	xn297l_set_mac(xn297l.mac);	
	
	//printf("xn297l init ok.\r\n");
	return TRUE;
}

volatile uint8_t delay_tick;

//---------------------------------------------------------------------------------------
//	Description	: 	xn297l send data
//	Parameters 	: 	src - data to be send, tx_len - src length, time_out - send time out
//	Return 		: 	true/false - send success/fail
//	Author 		: 	RAY
//	Date		:	2013 - 08 - 18
//---------------------------------------------------------------------------------------
BOOL xn297l_transceive (
	RF_DIR rf_dir, uint8_t pwr_ctrl, uint8_t t_out,
	uint8_t *tx_src, uint8_t tx_len,  
	uint8_t *rx_dst, uint8_t dst_len, uint8_t *rx_len
)
{
	BOOL flag = FALSE;

	if(pwr_ctrl)
	{
		/* rf wakeup first */
		xn297l_goto_wakeup();
	}

	xn297l_goto_stb3();
    BSPTimerDelay(50);
	
	/* pkt io config */
	//pkt_int_enable();
	
	/* rf host transfer */
	if(RF_TRANSFER == rf_dir)
	{
		/* rf goto tx state */
		xn297l_goto_tx();
		
		/* write data to tx payload fifo */
		xn297l_write_pld_fifo(tx_src, tx_len);

		/* clear rx fifo */
		xn297l_clear_rx_fifo();
		
		/* tx payload start */
		xn297l_tx_start();
	}
	else // for RF_RECEIVER
	{		
		/* rf goto rx state */
		xn297l_goto_rx();

#ifdef RF_ACK_PLD
		/* clear tx fifo */
		xn297l_clear_tx_fifo();
		
		/* write data to ack payload fifo */
		xn297l_write_ack_fifo(0, tx_src, tx_len);

		/* Write Ack Data twice for rf bug */
		xn297l_write_ack_fifo(0, tx_src, tx_len);
#endif /* RF_ACK_PLD */

		/* rx data start */
		xn297l_rx_start();
	}

	/* clear pkt int */
	//pkt_int_clear();
		
	/* wait ack signal */
	delay_tick = 0;
	
	while(delay_tick < t_out)
	{
	#ifdef _RF_HOST
		/* idle for low power */
		mcu_goto_idle();
	#endif

	    delay_1ms(1);

		/* wait for rf irq */
    //#ifndef RF_CE_PKT_SOFT
	//	if(0 == XN297L_PKT)
    //#endif /* RF_CE_PKT_SOFT */
		{
			uint8_t irq_flag;
			
			irq_flag = xn297l_get_irq_flag();
            //uart_send_byte(irq_flag);

        //#ifdef RF_CE_PKT_SOFT
            if(irq_flag != 0)
        //#endif /* RF_CE_PKT_SOFT */
            {
    			//printf("irq=%x.\r\n", (UINT16)irq_flag);
                //uart_send_byte(irq_flag);

    			/* rf host transfer */
    			if(RF_TRANSFER == rf_dir)
    			{
    				/* TX_DS = 1:rf get ack */
    				if(irq_flag & _TX_DS)
    				{
    				    //if(!(irq_flag & _MAX_RT))
    				    {
        					//printf("get ack.\r\n");
        					flag = TRUE;
        				
                        #ifndef RF_ACK_PLD
        					goto RF_ERR;
        				#endif
    					}
    				}
    				/* MAX_RT = 1:rf tx fail */
    				else// if(irq_flag & _MAX_RT)
    				{
    					goto RF_ERR;
    				}
    			}
    			else // for RF_RECEIVER
    			{
    				/* only get rx data */
    				if(irq_flag & _RX_DR)
    				{
    					//xn297l_clear_irq_flag(_RX_DR);
						RF_CE_SET_LOW;
    					
    					/* ack has not tx done */
    					delay_1ms(3); // time for ack
    					flag = TRUE;
    				}
    				else // err state
    				{
    					goto RF_ERR;
    				}
    			}
    			
    			/* get rf fifo data */
    			xn297l_fifo_read(rx_dst, dst_len, rx_len);

    			if(*rx_len == 0)
    			{
    			    flag = FALSE;
    			}
    			
    		RF_ERR:
    			/* clear rx fifo */
    			xn297l_clear_rx_fifo();

    			/* clear tx fifo */
    			xn297l_clear_tx_fifo();
    			
    			/* clear irq flag */
    			xn297l_clear_irq_flag(irq_flag);
    			break;
			}
		}
	}

	/* pkt io reset */
	//pkt_int_disable();
	//pkt_int_clear();

	xn297l_goto_stb3();
	BSPTimerDelay(10);

	/* rf goto idle state */
	xn297l_goto_stb1();
	BSPTimerDelay(10);

	if(pwr_ctrl)
	{
		/* rf goto sleep */
		xn297l_goto_sleep();
	}
	
	return flag;
}


void RF_Carrier(uint8_t ucChannel_Set)
{
    uint8_t BB_cal_data[]    = {0x0A,0x6D,0x67,0x9C,0x46}; 
    uint8_t RF_cal_data[]    = {0xF6,0x37,0x5D};
    uint8_t RF_cal2_data[]   = {0x45,0x21,0xEF,0xAC,0x5A,0x50};
    uint8_t Dem_cal_data[]   = {0xE1}; 								
    uint8_t Dem_cal2_data[]  = {0x0B,0xDF,0x02};      
    //uint8_t tmp;

#ifdef _SPI_HARDWARE
    serial_baud(xn297l_spi_no, SPEED_3M);
#else
    //spi_half_duplex_init();
    sRF_Configuration();
#endif /* _SPI_HARDWARE */
    
    //XN297L_CE = 0;
	RF_CE_SET_LOW;
	xn297l_write_1byte(REG_CONFIG | CMD_W_REGISTER, 0x8E);
    //xn297l_write_1byte(CMD_RST_FSPI, 0x5A); //Software Reset    			
    //xn297l_write_1byte(CMD_RST_FSPI, 0XA5);
    delay_1ms(200);
    xn297l_write_1byte(CMD_W_REGISTER + REG_RF_CH, ucChannel_Set);  //µ¥ÔØ²¨Æµµã	   
    //xn297l_read_1byte(REG_RF_CH, &tmp);
    //uart_send_byte(tmp);
    xn297l_write_1byte(CMD_W_REGISTER + REG_RF_SETUP, 0x3F);        //13dbm
    xn297l_write_bytes(CMD_W_REGISTER + REG_BB_CAL,    BB_cal_data,  sizeof(BB_cal_data));
    xn297l_write_bytes(CMD_W_REGISTER + REG_RF_CAL2,   RF_cal2_data, sizeof(RF_cal2_data));
    xn297l_write_bytes(CMD_W_REGISTER + REG_DEMOD_CAL, Dem_cal_data, sizeof(Dem_cal_data));
    xn297l_write_bytes(CMD_W_REGISTER + REG_RF_CAL,    RF_cal_data,  sizeof(RF_cal_data));
    xn297l_write_bytes(CMD_W_REGISTER + REG_DEM_CAL2,  Dem_cal2_data,sizeof(Dem_cal2_data));
    BSPTimerDelay(250);	

    while(1) BSPWatchDogReset();
}


