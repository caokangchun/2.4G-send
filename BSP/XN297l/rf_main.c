
#include "includes.h"

static uint32_t MAC2_4G=0;

uint32_t* GetMAC2_4G(void)
{
	return &MAC2_4G;
}



#ifdef _RF_HOST

extern BOOL USB_Key_Send(uint8_t *send_buff, UINT16 length, uint8_t type);

UART_RECV uart = {RECV_IDLE, 0};
RF_HOST rf_host = {0};
RING_CTRL uart_ring;
uint8_t uart_buffer[RING_SIZE] = {0};
uint8_t pid_mask = 0;


static bool SendDataBlueT(uint8_t mode);
static bool SendData2_4G(uint8_t mode);


/*---------------------------------------------------------------------------------------------------------*/
/* Function:                                                                                               */
/*               UART0_Send                                                                                */
/* Parameters:                                                                                             */
/*               byte - data to be send                                                                    */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to use UART0 send data                                               */
/*---------------------------------------------------------------------------------------------------------*/
void RF_Timer(void)
{
    delay_tick++;

//	key_trig_task();
//	beep_task();

//    sys_bat.time++;
    rf_host.idleout++;
    rf_host.tx_out++;
    rf_host.tx_time++;
    rf_host.same_out++;
    
    if (rf_host.same_out > 6500)
    {
        rf_host.same_out = 6500;
    }

#ifdef _BAR_UART
    if (uart.state != RECV_IDLE)
    {
        if (uart.time_out != 0)
        {
            uart.time_out--;
            
            if (0 == uart.time_out)
            {
                /* receive data timeout */
                if (RECV_DATA == uart.state)
                {
                    //rf_host.tx_out = 0;
                    //test_pin2 = !test_pin2;
                    if ((uart.index_tmp + 2) <= (uart_ring.buffer_size - RingBuffer_Data_Len(&uart_ring)))
                    {
                        RingBuffer_Write(&uart_ring, (uint8_t *)&uart.index_tmp, 2);//1);
                        RingBuffer_Write(&uart_ring, uart.buff_tmp, uart.index_tmp);
                    }

                    if (rf_host.beep & 0x01)
                    {
    					//beep_flag = 1;
    					//BEEP_ON;
//    					beep_enable(80);	//ckc, 扫描超时完成，提示成功
    					//MOTOR_ON;
					}
                }
                /* overflow data timeout */
                else if (RECV_STOP == uart.state)	//当数据超过环形缓冲区大小
                {
                    //uart.index = 0;
                }
                
                uart.state = RECV_IDLE;
            }
        }
    }
#endif /* _BAR_UART */
}

/*******************************************************************************
* Function Name  : RF_HostTout
* Description    : RF_HostTout
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL RF_HostTout(TIME_OUT mode)
{
	BOOL flag = FALSE;
	
	switch (mode)
	{
		case TX_OUT:
			if (rf_host.tx_out > RF_SEND_TOUT) // tx time out
			{
				// tx timeout
				//printf("tx tout.\r\n");
				flag = TRUE;
			}
			else
			{
				if(rf_host.rx_init)
				{
					flag = TRUE;
				}
			}
			break;

		case IDLE_OUT:
			if (rf_host.idleout > rf_host.sleep_t) // idle time out
			{
				// goto sleep
				rf_host.status = RF_PWR_OFF;
				//printf("idle tout.\r\n");
				flag = TRUE;
			}
			break;

		case INIT_OUT:
			flag = TRUE;
			break;
			
		default:
			break;
	}

	return flag;
}

#define _OLD
#ifdef _OLD

//------------------------------------------------------------------------
//	Description	: 	UART data to usb send
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 03 - 11
//------------------------------------------------------------------------
void usb_tx_delay(uint8_t *dat_buf, UINT16 dat_len)
{
    #define HID_MS  1 // MS
    
    UINT16 i;

    for(i = 0; i < dat_len; i++)
    {
        if(dat_buf[i] == 0xFF)
        {
            i += 3;
            // 1 GBK = 12 HID_MS
            delay_1ms(20*HID_MS);
        }
        else
        {
            // 1 ASCII = 2 HID_MS
            delay_1ms(2*HID_MS);
        }

        BSPWatchDogReset();
    }
}

//------------------------------------------------------------------------
//	Description	: 	UART data to usb send
//	Parameters 	: 	none
//	Return 		: 	none
//	Author 		: 	RAY
//	Date 		: 	2014 - 03 - 11
//------------------------------------------------------------------------
BOOL RF_HostTxDatas(uint8_t mode)			//优先级     蓝牙   USB  2.4G
{
	//蓝牙
	if(BT_LINK_IN) // BT connected
	{
		return SendDataBlueT(mode);
	}
		
	//2.4G
	return SendData2_4G(mode);
}


static bool SendDataBlueT(uint8_t mode)
{
#ifdef _BT_MODULE						

    if (mode != INIT_MODE)
    {
        if (BT_LINK_IN) // BT connected
        {
			if (BT_TxTPDatas(&uart.buff[HEAD_LEN], uart.index-HEAD_LEN))
			{
				rf_host.idleout = 0; // clear idle cnt
				return TRUE;
			}
        }
    }
	return FALSE;
#endif /* _BT_MODULE */
}


//无线发送
static uint8_t pkt_sid = 0x11;	//表示没有上传
uint8_t getPkt_sid(void)
{
	return pkt_sid;
}

unsigned char crc_high_first(unsigned char *ptr, unsigned char len); 
static bool SendData2_4G(uint8_t mode)
{
	BOOL status = FALSE;
	uint8_t ack_tmp[4];
	uint8_t ack_len;

	uint8_t receive_status[4]={0};
	uint8_t receive_status_len=0;


#ifdef RF_250KBPS
	uint8_t i;
	uint8_t full_bag;
	uint8_t last_len;
	uint8_t tx_buf[RF_FIFO_MAX];
	uint8_t *tx_index = NULL;
#endif /* RF_250KBPS */

	static uint8_t pkt_id = 0;

//    uart.buff[HID_NO] = rf_host.pid;	// host id
    uart.buff[HID_NO] = pkt_sid;	// sid
	/* clear tx timeout */
	rf_host.tx_out = 0;

	/* packet id counter */
//	uart.buff[PID_NO] = pkt_id & 0x1F; // 5bit
	uart.buff[PID_NO] = pkt_id & 0x0F;
	pkt_id++;
	
	/* pid control bit */
	if((!pid_mask) && (rf_host.add_type != 0))
	{
        uart.buff[PID_NO] |= DAT_LEFT(rf_host.add_type, 5);
	}

    uart.buff[LAN_NO] = uart.ex_info;

    
#ifdef RF_250KBPS

//RF_SNED_DATA:

	status = FALSE;

	full_bag = uart.index / (RF_FIFO_MAX - 1);		// ckc ，first to be bag id
	last_len = uart.index % (RF_FIFO_MAX - 1);

	//第一个数据包，需要添加crc，否则可能出现数据错误
	//crc1放在第一个字节
	if(full_bag)
	{
		uart.buff[0] = crc_high_first(&uart.buff[1], RF_FIFO_MAX - 2);	//16-bagid-crc
	}
	else
	{
		uart.buff[0] = crc_high_first(&uart.buff[1], last_len);	
	}
	
    //装载CRC8 
    uart.buff[uart.index-1] = crc_high_first(&uart.buff[0], uart.index-1);

	for(i = 0; i < full_bag; i++)
	{
	RF_SNED_DATA1:
		
		
		tx_index = &uart.buff[i*(RF_FIFO_MAX-1)];
		tx_buf[0] = i + 1; // first to be bag id
		memcpy(&tx_buf[1], tx_index, RF_FIFO_MAX-1);
		
		status = rf_transceiver(RF_TRANSFER, RF_TX_TOUT, tx_buf, RF_FIFO_MAX, \
								ack_tmp, sizeof(ack_tmp), &ack_len);
		if(status) // tx ok and get ack data
		{
			//status = !memcmp(ack_tmp, &rf_ack[i][0], ACK_C);
			status = !memcmp(ack_tmp, &rf_ack[0][0], ACK_C);
            if(!status) // ack fail, re-transmit
            {
                if(!RF_HostTout(TX_OUT))
                {
                    //goto RF_SNED_DATA;
                    goto RF_SNED_DATA1;
                }
                else
                {
                	break; // RF tx timeout
                }
            }
            else
            {
            	/* clear tx timeout */
				rf_host.tx_out = 0;
				
            	continue; // RF get ack OK
            }
		}
		else
		{
			break; // RF tx timeout
		}
	}

	if((status == TRUE) || (full_bag == 0)) // full bag send ok, or only need tx last data
	{
		if(last_len != 0)
		{
		RF_SNED_DATA2:
		
			tx_index = &uart.buff[i*(RF_FIFO_MAX-1)];
			tx_buf[0] = i + 1; // first to be bag id
			memcpy(&tx_buf[1], tx_index, last_len);
			
			status = rf_transceiver(RF_TRANSFER, RF_TX_TOUT, tx_buf, last_len + 1, \
									ack_tmp, sizeof(ack_tmp), &ack_len);
									
			if(status) // tx ok and get ack data
			{
		    	if(mode == INIT_MODE)
		    	{
		    	    UINT32 *mac32 = NULL;
		    	    
		    		memcpy(xn297l.mac, ack_tmp, 4);
					//at24cxx_write(RF_MAC, xn297l.mac, 4);
					mac32 = (UINT32 *)&ack_tmp;
					
//					user_cfg_mod(RF_MAC, *mac32);
		    		status = TRUE;
					MAC2_4G = *mac32;
		    	}
		    	else // DATA MODE
		    	{
					//status = !memcmp(ack_tmp, &rf_ack[i][0], ACK_C);
					status = !memcmp(ack_tmp, &rf_ack[0][0], ACK_C);
				}
		        
		        if(!status) // ack fail, re-transmit
		        {
		            if(!RF_HostTout(TX_OUT))
		            {
		                //goto RF_SNED_DATA;
		                goto RF_SNED_DATA2;
		            }
		        }
			}						
		}
	}
#endif /* RF_250KBPS */

	if(status) // rx ack ok, tx success
	{
		rf_host.tx_out = 0;

		//status = rf_transceiver(RF_RECEIVER, 250, ack_tmp, sizeof(ack_tmp), receive_status, RF_FIFO_MAX, &receive_status_len);		
		status = rf_transceiver(RF_TRANSFER, RF_TX_TOUT, tx_buf, last_len + 1, \
											receive_status, sizeof(receive_status), &receive_status_len);

		if(status)
		{
			if((0xaa == receive_status[0]) && (0x55 == receive_status[3]))
			{
				if(receive_status[1] == uart.buff[PID_NO])	//判断返回的数据是否为本次响应
				{
					//分析spid
					rf_host.idleout = 0; // clear idle cnt
					pkt_sid = receive_status[1];   //成功，记录sid					
				}
				else	
				{
					status = FALSE;
				}
				
			}
			else
			{
				status = FALSE;
			}
		}
		else
		{
			//__nop();
			status = FALSE;
		}
	}

	return status;
}


bool SendCmd2_4G(uint8_t sid,uint8_t crc)
{
	BOOL status = FALSE;
	uint8_t ack_tmp[4];
	uint8_t ack_len;

#ifdef RF_250KBPS
	uint8_t i;

	uint8_t tx_buf[RF_FIFO_MAX];
	uint8_t *tx_index = NULL;
#endif /* RF_250KBPS */


    
	rf_host.tx_out = 0;
	status = FALSE;




	{

		{
		RF_SNED_DATA2:
		
			tx_buf[0] = 1; // first to be bag id
			tx_buf[7] = 0xfe;
			tx_buf[8] = 0xef;
			tx_buf[9] = sid;
			tx_buf[10] = crc;

			
			status = rf_transceiver(RF_TRANSFER, RF_TX_TOUT, tx_buf, 11, \
									ack_tmp, sizeof(ack_tmp), &ack_len);
									
			if(status) // tx ok and get ack data
			{
				status = !memcmp(ack_tmp, &rf_ack[0][0], ACK_C);
				
		        
		        if(!status) // ack fail, re-transmit
		        {
		            if(!RF_HostTout(TX_OUT))
		            {
		                //goto RF_SNED_DATA;
		                goto RF_SNED_DATA2;
		            }
		        }
			}						
		}
	}


	if(status) // rx ack ok, tx success
	{
		rf_host.idleout = 0; // clear idle cnt
		usb_tx_delay(&uart.buff[HEAD_LEN], uart.index-HEAD_LEN);
	}

	return status;
}


#endif

/*******************************************************************************
* Function Name  : BAR_SameCheck
* Description    : BAR_SameCheck
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BAR_SameCheck(void)
{
	BOOL flag = FALSE;
	
#if 0
	if(rf_host.same_flag)
	{
		UINT16 len;
		
		len = uart.index - 3; // no cnt & pkt & pid

		if(uart.index_last != len)
		{
			uart.index_last = len;
		}
		else
		{
			if(rf_host.same_out < rf_host.same_time)
			{
				if(!memcmp(uart.buff_last, &uart.buff[3], len))
				{
					flag = TRUE;
				}
			}
		}

		if(!flag) // new barcode
		{
			memcpy(uart.buff_last, &uart.buff[3], len);
			
			// clear time out cnt
			rf_host.same_out = 0;
		}
	}
#endif /* 0 */

	return flag;
}




/*******************************************************************************
* Function Name  : RF_PairForRx
* Description    : RF_PairForRx
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
bool RF_PairForRx(void)
{
    BOOL flag = 0;
	uint8_t tx_cnt = 0;

	rf_set_pipe(PIPE_INDEX_MAX);
	rf_set_ch();
	rf_set_mac((uint8_t *)rf_def_para);

	BT_Disconnect();
	

	//uart_rx_int_dis();
//	while ( BT_LINK_IN )	//ckc
    {
        BSPWatchDogReset();
    }
//	BT_PairClear();
	
	rf_host.idleout = 0;

	while (1)
	{
		if ( RF_HostTxDatas( INIT_MODE ) )
		{
            flag = 1;
			break;
		}
		else
		{
			if ( ++tx_cnt > 12 )
			{
				tx_cnt = 0;
				
				//if(rf_host.beep)
				{
//					beep_ctrl(1, 80);
                    delay_1ms(80);
				}
			}
		}
		
        if ( BT_LINK_IN )
        {
            //flag = 1;
            break;
        }

		if ( rf_host.idleout > 30*1000 ) // 30S
		{
			break;
		}

        #if 0
		if(uart_rx_int_flag)
		{
            uart.index_tmp = 0;
		    uart.state = RECV_STOP;
            uart.time_out = T_OUT_MAX;
			break;
		}
        #endif /* 0 */
	}

	if ( flag )
	{
        delay_1ms(80);
//        beep_ctrl(1, 80);
        delay_1ms(30);
//        beep_ctrl(1, 80);
        rf_host.idleout = 0;
	}
	
	rf_host.rx_init = FALSE;
	rf_set_pipe((xn297l.mac[3] + 1) % PIPE_INDEX_MAX);
	rf_set_ch();
	rf_set_mac(xn297l.mac);


	//uart_rx_int_en();
	uart.data_ok = 0;
	
	return flag;	//ckc
}



#if 1//def _CN_SUPPORT
/*******************************************************************************
* Function Name  : UTF8_ConvertGBK
* Description    : UTF8_ConvertGBK
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
void UTF8_ConvertGBK(uint8_t *pbuf, UINT16 len)
{
    UINT32 unicode;
    uint8_t gbk[2];
    UINT16 i = 0;
    signed char ret0, ret1; 

    while(1)
    {
        BSPWatchDogReset();
        
        ret0 = utf8_mbtowc((ucs4_t *)&unicode, &pbuf[i], 6);
        if(ret0 > 0) // Decode OK
        {
            if((ret0 == 2) || (ret0 > 3)) // Not Support	//对于2个字节的UTF8的编码不支持，或大于3个字节的UTF8的编码不支持
            {
                if((ret0 == 2) && (pbuf[i] == 0xC2))
                {
                    pbuf[i] = 0;
                }
                else
                {
                    memset(&pbuf[i], 0, ret0);
                }
            }
            else if(ret0 == 3) // Transform to GBK			//支持中文转换，中文刚好是3个字节 UTF8编码
            {
                ret1 = cp936_wctomb(gbk, unicode, 6);
                if(ret1 == 2) // Decode OK
                {
                    if(!((gbk[0] == 0xFF) && (gbk[1] == 0xFF)))
                    {
                        pbuf[i] = 0xFF;
                        pbuf[i+1] = gbk[0];
                        pbuf[i+2] = gbk[1];
                    }
                    else // value error
                    {
                        memset(&pbuf[i], 0, ret0);
                    }
                }
                else // Decode Error
                {
                    memset(&pbuf[i], 0, ret0);
                }
            }
            else if(ret0 == 1) // To be ASCII
            {
                // Do Nothing
            }
            
            i += ret0;
        }
        else // Decode Error
        {
            pbuf[i] = 0;
            i++;
        }

        if(i > len)
        {
            break;
        }
    }
}

/*******************************************************************************
* Function Name  : Alipay_Ticket
* Description    : Alipay_Ticket
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
void Alipay_Ticket(uint8_t *pbuf, UINT16 *len)
{
    //const uint8_t h_flag[3] = {0xEF, 0xBB, 0xBF};
    const uint8_t t_flag[3] = {0xFF, 0xA1, 0xFA};
    UINT16 i;
    uint8_t temp, head = 0;
    UINT16 pos_1, pos_2 = 0, pos_3 = 0, pos_4 = 0, pos_A = 0;

    for(i = 0; i < *len; i++)
    {
        if(head == 0)
        {
            if((pbuf[i] == '1') && (pbuf[i+1] == '1'))
            {
                pos_1 = i;
                i += 1;
                head = 1;
            }
        }
        else
        {
            if(!memcmp(t_flag, &pbuf[i], 3))
            {
                temp = pbuf[i+3];
                
                if(temp == '2')
                {
                    pos_2 = i+3;
                    i += 3;
                }
                else if(temp == '3')
                {
                    pos_3 = i+3;
                    i += 3;
                }
                else if(temp == '4')
                {
                    pos_4 = i+3;
                    i += 3;
                }
                else if(temp == 'A')
                {
                    pos_A = i+3;
                    i += 3;
                }
            }
        }
    }

    if((pos_2 && pos_3 && pos_4 && pos_A) && \
    (pos_1 < pos_2) && (pos_2 < pos_3) && (pos_3 < pos_4) && (pos_4 < pos_A))
    {
        // Clear "11"
        pbuf[pos_1+0] = 0;
        pbuf[pos_1+1] = 0;

        // "→2" to '\t'
        pbuf[pos_2-3] = 0;
        pbuf[pos_2-2] = 0;
        pbuf[pos_2-1] = 0;
        pbuf[pos_2-0] = '\t';

        // "→3" to '\t'
        pbuf[pos_3-3] = 0;
        pbuf[pos_3-2] = 0;
        pbuf[pos_3-1] = 0;
        pbuf[pos_3-0] = '\t';

        // "→4" to '\t'
        pbuf[pos_4-3] = 0;
        pbuf[pos_4-2] = 0;
        pbuf[pos_4-1] = 0;
        pbuf[pos_4-0] = '\t';

        // "→A" last discard
        *len = pos_A - 3;
    }
}

/*******************************************************************************
* Function Name  : Data_Adjustment0
* Description    : Data_Adjustment0
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
void Data_Adjustment0(void)
{
//     UTF8_ConvertGBK(&uart.buff[HEAD_LEN], uart.index);		//HEAD_LEN	数据起始下标。 转换成 0xff + GBK

    if(rf_host.ticket)	//????
    {
        Alipay_Ticket(&uart.buff[HEAD_LEN], &uart.index);
    }
    
    if(uart.buff[HEAD_LEN+uart.index-1] == ENTER) // Postfix = 0x0D				
    {
        uart.cmd_type = rf_cmd_judge(&uart.buff[HEAD_LEN], uart.index-1);
    }
    else if(uart.buff[HEAD_LEN+uart.index-2] == ENTER) // Postfix = 0x0D+0x0A	
    {
        uart.cmd_type = rf_cmd_judge(&uart.buff[HEAD_LEN], uart.index-2);
        uart.index--; // Delete 0x0A
    }
    else // Postfix = None		
    {
        uart.cmd_type = rf_cmd_judge(&uart.buff[HEAD_LEN], uart.index);		//ckc，CMD_NONE
        uart.buff[HEAD_LEN+uart.index] = ENTER;		//ckc，手动添加后缀
        uart.index++; // Add Enter 0x0D				
    }
    
    {
    #if 0
        UINT16 crc16;
        crc16 = calc_crc16_ccitt(&uart.buff[HEAD_LEN], uart.index);
        uart.buff[HEAD_LEN+uart.index] = (crc16 >> 8) & 0xFF;
        uart.buff[HEAD_LEN+uart.index+1] = crc16 & 0xFF;
        uart.index += 2;
    #endif /* 0 */
    }
    
    uart.index += HEAD_LEN;
}
#endif /* _CN_SUPPORT */

/*******************************************************************************
* Function Name  : RF_HostTxFunc
* Description    : RF_HostTxFunc
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL RF_HostTxFunc(void)
{
	BOOL status = FALSE;

#ifdef _BAR_UART
	if(RingBuffer_Data_Len(&uart_ring))
	{
	    __disable_irq();
        RingBuffer_Read(&uart_ring, (uint8_t *)&uart.index, 2);//1);
	    RingBuffer_Read(&uart_ring, &uart.buff[HEAD_LEN], uart.index);		//ckc 从环形区读取到uart.buff.      //实际上数据从uart.buff_temp ->uart_ring->&uart.buff[HEAD_LEN]

        __enable_irq();
		
        Data_Adjustment0();

        uart.data_ok = TRUE;	//收到数据，编码转换(UTF8->GBK)，增加后缀等完成
	}
#endif /* _BAR_UART */
	
	if ( uart.data_ok )
	{	
	    uart.buff[0] = 0xFF;				//ckc 0xff开头

	    //添加1字节 crc8校验
		uart.index++;
	    
		/* tx total bytes */
		uart.buff[1] = uart.index / 256;	//ckc 后续数据长度
        uart.buff[2] = uart.index % 256;	

		/* tx host id number */
		uart.buff[HID_NO] = rf_host.pid;	
	
		BSPWatchDogReset();

		if ( rf_host_cmd(uart.buff) != TRUE )		//ckc 解析各种命令？
		{
				
			if ( BAR_SameCheck() != TRUE )
			{
				if ( RF_HostTxDatas(DATA_MODE) == TRUE )	//ckc 数据发送？
				{
					status = TRUE;
				}
				else
				{
					
				}
				
				// enable data rx
				uart.data_ok = 0;
			}
		}

		BSPWatchDogReset();
        uart.data_ok = 0;
	}
	
	else	//无数据ckc
	{	

	}
	
	return status;
}

/**/



/*******************************************************************************
* Function Name  : Wireless_Main
* Description    : Wireless_Main
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
bool Wireless_Main(void)
{
	bool status = FALSE;
    switch ( rf_host.status )
    {
        case RF_ONLINE:
            /* rf tx uart data */
            status = RF_HostTxFunc();

            break;
    
        case RF_PWR_OFF:

            break;
            
        default:
            break;
    }
	return status;
}



#endif

