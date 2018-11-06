
#include "includes.h"
#include "UART2.h"



/*用于设置蓝牙名字 add by ckc*/
static bool BT_SetName();

#ifdef _BT_MODULE

#define BT_UART     UART2 // BT UART2/115200


#define _BT_RSSI_CLK	RCC_APB2Periph_GPIOC
#define _BT_RSSI_PORT   GPIOC
#define _BT_RSSI_PIN    GPIO_Pin_13
#define BT_RSSI_IN      GPIO_ReadInputDataBit(_BT_RSSI_PORT, _BT_RSSI_PIN)

#define _BT_DISC_CLK	RCC_APB2Periph_GPIOB
#define _BT_DISC_PORT   GPIOB
#define _BT_DISC_PIN    GPIO_Pin_5
#define BT_DISC_H       GPIO_SetBits(_BT_DISC_PORT, _BT_DISC_PIN);
#define BT_DISC_L       GPIO_ResetBits(_BT_DISC_PORT, _BT_DISC_PIN);

#define _BT_LINK_CLK	RCC_APB2Periph_GPIOC
#define _BT_LINK_PORT   GPIOC
#define _BT_LINK_PIN    GPIO_Pin_14
#define BT_LINK_IN      GPIO_ReadInputDataBit(_BT_LINK_PORT, _BT_LINK_PIN)




#define BARCODE_SCANNER_NAME		"SM_CODE"





BT_CTRL bt = {BT_IDLE, 0};
uint8_t bt_mac[12] = "XxXxXxXxXxXx";

// BT ADDR Get Command
char *bt_mac_get     = "AT+ADDR\r\n";
// BT Name Configuration
char *bt_name_get    = "AT+NAME\r\n";
char *bt_name_resp   = "\r\n+NAME=BARCODE SCANNER\r\n\r\nOK\r\n";
char *bt_name_set    = "AT+NAME=BARCODE SCANNER\r\n";
// BT Mode Configuration
char *bt_mode_get    = "AT+MODE\r\n";
char *bt_mode_resp1  = "\r\n+MODE=1\r\n\r\nOK\r\n";
char *bt_mode_set1   = "AT+MODE=1\r\n";
char *bt_mode_resp2  = "\r\n+MODE=2\r\n\r\nOK\r\n";
char *bt_mode_set2   = "AT+MODE=2\r\n";
// BT Piocfg Configuration
char *bt_piocfg_get  = "AT+PIOCFG\r\n";
char *bt_piocfg_resp = "\r\n+PIOCFG=1,1\r\n\r\nOK\r\n";
char *bt_piocfg_set  = "AT+PIOCFG=1,1\r\n";
// BT Hiddly Configuration
char *bt_hiddly_get  = "AT+HIDDLY\r\n";
char *bt_hiddly_resp = "\r\n+HIDDLY=2\r\n\r\nOK\r\n";
char *bt_hiddly_set  = "AT+HIDDLY=2\r\n";
// BT tp Configuration
char *bt_tp_get      = "AT+TPMODE\r\n";
char *bt_tp_resp     = "\r\n+TPMODE=1\r\n\r\nOK\r\n";
char *bt_tp_set      = "AT+TPMODE=1\r\n";
// BT Disconnect Command
char *bt_disc_cmd    = "AT+DISC\r\n";
// BT Set OK Response
char *bt_resp_ok     = "\r\nOK\r\n";
// BT Send Data OK
char *bt_send_ok     = "\r\n+HIDSEND\r\n";
// BT HID Send Data
char *bt_hid_send    = "AT+HIDSEND=";
// BT Pair Lcear
char *bt_pair_clr    = "AT+PLIST=1\r\n";
// BT hid connect
char *bt_hid_rec     = "AT+HIDCONN\r\n";
char bt_hid_conn[26] = "AT+HIDCONN=XxXxXxXxXxXx\r\n";
// BT SPP connect
char *bt_spp_rec     = "AT+SPPCONN\r\n";
char bt_spp_conn[26] = "AT+SPPCONN=XxXxXxXxXxXx\r\n";;


char *bt_uart_test   = "AT\r\n";
char *bt_hsm_set     = "AT+HSM=0\r\n";
char *bt_baud_set    = "AT+BAUD=115200\r\n";
char *bt_restore	 = "AT+RESTORE\r\n";

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
void BT_Timer(void)
{
    bt.rx_tout++;
    
    if (bt.state != BT_IDLE)
    {
        if (bt.time_out != 0)
        {
            bt.time_out--;
            
            if (0 == bt.time_out)
            {
                /* receive data timeout */
                if (BT_DATA == bt.state)
                {                    
                    bt.data_ok = TRUE;
                }
                
                bt.state = BT_IDLE;
            }
        }
    }
    
    if (bt.init)
    {
        if (BT_LINK_IN != bt.linked)
        {
            bt.linked = BT_LINK_IN;
//            beep_enable(50);
            rf_host.idleout = 0;
        }
    }
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
void BT_UART_Rx(uint8_t dat)
{	
	bt.time_out = BT_OUT_MAX;

	switch (bt.state)
	{
		case BT_IDLE:
			if (bt.data_ok != TRUE)
			{
				bt.index_tmp = 0;
				bt.state = BT_DATA;
			}
			else
			{
				bt.state = BT_STOP;
				break;
			}
			
		case BT_DATA:
			if (bt.index_tmp >= BT_BUFF)
			{
				bt.state = BT_STOP;
			}
			else
			{
				bt.buff_tmp[bt.index_tmp++] = dat;
			}
			break;
			
		case BT_STOP:
			/* discard data */
			break;

		default:
			bt.state = BT_IDLE;
			break;
	}
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BT_Command(uint8_t *cmd_dat, uint8_t cmd_len, uint8_t *resp_dat, uint8_t *resp_len)
{
    BOOL flag = FALSE;
    
    bt.rx_tout = 0;
    bt.data_ok = 0;
    bt.state = BT_IDLE;
    
    UART_SendDatas(BT_UART, cmd_dat, cmd_len);
    
    while (bt.rx_tout < 500)
    {
	  	BSPWatchDogReset();
		
        if (bt.data_ok)
        {
            flag = TRUE;
            *resp_len = bt.index_tmp;
            memcpy(resp_dat, bt.buff_tmp, bt.index_tmp);
            break;
        }
    }

    return flag;
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BT_PairClear(void)
{
    BOOL flag;
    
    flag = BT_Command((uint8_t *)bt_pair_clr, strlen(bt_pair_clr), bt.resp_buf, &bt.resp_len);
    if (!flag)
        flag = BT_Command((uint8_t *)bt_pair_clr, strlen(bt_pair_clr), bt.resp_buf, &bt.resp_len);

    if (flag)
    {
        if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))
        {
            flag = FALSE;
        }
    }

    return flag;
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BT_Reconnect(void)
{
    BOOL flag;
    
    if (rf_host.bt_mod == 0) // HID
    {
        flag = BT_Command((uint8_t *)bt_hid_rec, strlen(bt_hid_rec), bt.resp_buf, &bt.resp_len);
        if (!flag)
            flag = BT_Command((uint8_t *)bt_hid_rec, strlen(bt_hid_rec), bt.resp_buf, &bt.resp_len);
    }
    else // SPP
    {
        flag = BT_Command((uint8_t *)bt_spp_rec, strlen(bt_spp_rec), bt.resp_buf, &bt.resp_len);
        if (!flag)
            flag = BT_Command((uint8_t *)bt_spp_rec, strlen(bt_spp_rec), bt.resp_buf, &bt.resp_len);
    }
    
    if (flag)
    {
        if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))
        {
            flag = FALSE;
        }
    }

    return flag;
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BT_ConnectReq(void)
{
    BOOL flag;
    
    BT_Disconnect();

    if (rf_host.bt_mod == 0) // HID
    {
        flag = BT_Command((uint8_t *)bt_hid_conn, strlen(bt_hid_conn), bt.resp_buf, &bt.resp_len);
        if (!flag)
            flag = BT_Command((uint8_t *)bt_hid_conn, strlen(bt_hid_conn), bt.resp_buf, &bt.resp_len);
    }
    else
    {
        flag = BT_Command((uint8_t *)bt_spp_conn, strlen(bt_spp_conn), bt.resp_buf, &bt.resp_len);
        if (!flag)
            flag = BT_Command((uint8_t *)bt_spp_conn, strlen(bt_spp_conn), bt.resp_buf, &bt.resp_len);
    }
    
    if (flag)
    {
        if (memcmp(bt.resp_buf, bt_resp_ok, strlen(bt_resp_ok)))
        {
            flag = FALSE;
        }
    }

    return flag;
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
void BT_Disconnect(void)
{
	BT_DISC_H;
    delay_1ms(10);

    if (BT_LINK_IN) // bt is connected
    {
        BT_DISC_L;
        delay_1ms(100);
        BT_DISC_H;
        delay_1ms(100);
    }
}

static void GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//MM32使用PC14 PC15 使用要注意下 如果做GPIO的话初始化要加入如下语句
	RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_RTCEN;
    RCC->BDCR &= ~RCC_BDCR_LSEON; 		
	
	RCC_APB2PeriphClockCmd(_BT_RSSI_CLK | _BT_DISC_CLK | _BT_LINK_CLK, ENABLE);
	
	// BT disconnect pin configuration
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_InitStructure.GPIO_Pin  = _BT_DISC_PIN;
    GPIO_Init(_BT_DISC_PORT, &GPIO_InitStructure);
	
    // BT rssi pin configuration
    GPIO_InitStructure.GPIO_Pin  = _BT_RSSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(_BT_RSSI_PORT, &GPIO_InitStructure);	
	
	// BT connect pin configuration
    GPIO_InitStructure.GPIO_Pin  = _BT_LINK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(_BT_LINK_PORT, &GPIO_InitStructure);	
	
	
}

/*ckc 添加到串口中断，用于接收蓝牙返回的消息*/
static void GetBTData(void)
{
	BT_UART_Rx(UART_ReceiveData(UART2));
}

/*******************************************************************************
* Function Name  : Bat_LowDetect
* Description    : Bat_LowDetect
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static bool SetBAUD(uint32_t baud)
{
	uint8_t tOut=10;
	bool flag;
	
	flag = BT_Command((uint8_t *)bt_baud_set, strlen(bt_baud_set), bt.resp_buf, &bt.resp_len);		//旧波特率
	if(!flag)
	{
		flag = BT_Command((uint8_t *)bt_baud_set, strlen(bt_baud_set), bt.resp_buf, &bt.resp_len);		//旧波特率
	}
	if(flag)	
	{
		if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))
		{
			flag = FALSE;
		}
	}
	
	
	if(flag)	//通过第一步设置
	{		
		UART_Configuration(BT_UART, baud);	//设置为新波特率
		
		while(tOut--)		//循环10次   10*500 = 5000ms
		{
			delay_1ms(500);
			
			flag = BT_Command((uint8_t *)bt_uart_test, strlen(bt_uart_test), bt.resp_buf, &bt.resp_len);		//新波特率  AT测试
			if(!flag)
			{
				flag = BT_Command((uint8_t *)bt_uart_test, strlen(bt_uart_test), bt.resp_buf, &bt.resp_len);		//新波特率	AT测试
			}	
			if(flag)	//有返回
			{
				if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))	//比较  
				{	
					flag = FALSE;			//不相等
				}
				else	
				{
					return TRUE;			//相等							
				}
			}		
		}
		
	}
	
	return flag;
}

BOOL BT_Initial(void)
{
    BOOL flag;
	
	//引脚初始化
	GpioInit();
    
	/* BT UART2 Configuration */
    UART_Configuration(BT_UART, 115200);
	
	UART2AddReadISR(GetBTData);
	
    //UART_IRQConfig(BT_UART, 2);
    
    BT_Disconnect();
	
//	BT_Command((uint8_t *)bt_baud_set, strlen(bt_baud_set), bt.resp_buf, &bt.resp_len);	//波特率
//	UART_Configuration(BT_UART, 115200);
	
//	flag = BT_Command((uint8_t *)bt_restore, strlen(bt_restore), bt.resp_buf, &bt.resp_len);	//恢复出厂
//	
//    flag = BT_Command((uint8_t *)bt_uart_test, strlen(bt_uart_test), bt.resp_buf, &bt.resp_len);
//    while (!flag)
//	{
//		flag = BT_Command((uint8_t *)bt_uart_test, strlen(bt_uart_test), bt.resp_buf, &bt.resp_len);
//		delay_1ms(500);
//	}
	





//SetBAUD(460800);






    flag = BT_Command((uint8_t *)bt_mac_get, strlen(bt_mac_get), bt.resp_buf, &bt.resp_len);
    if (!flag)
        flag = BT_Command((uint8_t *)bt_mac_get, strlen(bt_mac_get), bt.resp_buf, &bt.resp_len);
        
    if(flag) // Get BT ADDR OK
    {
        memcpy(bt_mac, &bt.resp_buf[8], 12);
    }
    else
    {
        //return 0;
    }
    
    if(flag) // BT ADDR OK
    {
		BT_SetName();	
    }
    
    if (flag) // BT Name OK
    {
        flag = BT_Command((uint8_t *)bt_mode_get, strlen(bt_mode_get), bt.resp_buf, &bt.resp_len);
        if (flag) // Get BT Mode OK
        {
        #if 1//def _BT_CONF
            if (rf_host.bt_mod == 0) // HID
            {
                if (memcmp(bt.resp_buf, bt_mode_resp2, strlen(bt_mode_resp2))) // Not User Mode
                {
                    flag = BT_Command((uint8_t *)bt_mode_set2, strlen(bt_mode_set2), bt.resp_buf, &bt.resp_len);
                    if (flag)
                    {
                        if (memcmp(bt.resp_buf, bt_resp_ok, strlen(bt_resp_ok)))
                        {
                            flag = FALSE;
                        }
                    }
                }
            }
            else // SPP
            {
                if (memcmp(bt.resp_buf, bt_mode_resp1, strlen(bt_mode_resp1))) // Not User Mode
                {
                    flag = BT_Command((uint8_t *)bt_mode_set1, strlen(bt_mode_set1), bt.resp_buf, &bt.resp_len);
                    if (flag)
                    {
                        if (memcmp(bt.resp_buf, bt_resp_ok, strlen(bt_resp_ok)))
                        {
                            flag = FALSE;
                        }
                    }
                }
            }
        #endif /* _BT_CONF */
        }
    }

    if (flag) // BT Mode OK
    {
        flag = BT_Command((uint8_t *)bt_piocfg_get, strlen(bt_piocfg_get), bt.resp_buf, &bt.resp_len);
        if (flag) // Get BT Piocfg OK
        {
        #if 1//def _BT_CONF
            if (memcmp(bt.resp_buf, bt_piocfg_resp, bt.resp_len)) // Not User Piocfg
            {
                flag = BT_Command((uint8_t *)bt_piocfg_set, strlen(bt_piocfg_set), bt.resp_buf, &bt.resp_len);
                if (flag)
                {
                    if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))
                    {
                        flag = FALSE;
                    }
                }
            }
        #endif /* _BT_CONF */
        }
    }

    if (flag) // BT Piocfg OK
    {
        flag = BT_Command((uint8_t *)bt_hiddly_get, strlen(bt_hiddly_get), bt.resp_buf, &bt.resp_len);
        if (flag) // Get BT Hiddly OK
        {
        #if 1//def _BT_CONF
            if (memcmp(bt.resp_buf, bt_hiddly_resp, bt.resp_len)) // Not User Piocfg
            {
                flag = BT_Command((uint8_t *)bt_hiddly_set, strlen(bt_hiddly_set), bt.resp_buf, &bt.resp_len);
                if (flag)
                {
                    if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))
                    {
						
                        flag = FALSE;
                    }
                }
            }
        #endif /* _BT_CONF */
        }
    }

    if (flag) // BT Mode OK
    {
        flag = BT_Command((uint8_t *)bt_tp_get, strlen(bt_tp_get), bt.resp_buf, &bt.resp_len);
        if (flag) // Get BT Piocfg OK
        {
        #if 1//def _BT_CONF
            if (memcmp(bt.resp_buf, bt_tp_resp, bt.resp_len)) // Not User Piocfg
            {
                flag = BT_Command((uint8_t *)bt_tp_set, strlen(bt_tp_set), bt.resp_buf, &bt.resp_len);
                if (flag)
                {
                    if (memcmp(bt.resp_buf, bt_resp_ok, bt.resp_len))
                    {
                        flag = FALSE;
                    }
                }
            }
        #endif /* _BT_CONF */
        }
    }
	
	

    bt.init = 1;
    return flag;
}

/*******************************************************************************
* Function Name  : bt_tx_data
* Description    : bt_tx_data
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BT_TxTPDatas(uint8_t *pdata, UINT16 len)
{
    BOOL flag = FALSE;
    
    bt.rx_tout = 0;
    bt.data_ok = 0;
    bt.state = BT_IDLE;

    UART_SendDatas(BT_UART, pdata, len);

    if (rf_host.bt_mod == 0) // HID
    {
        while (bt.rx_tout < RF_SEND_TOUT*5)
        {
    	  	BSPWatchDogReset();
    		
            if (bt.data_ok)
            {
                uint8_t len_tmp = strlen(bt_send_ok);
                
                if ((bt.index_tmp == len_tmp) && \
                   (!memcmp(bt.buff_tmp, bt_send_ok, len_tmp)))
                {
                    flag = TRUE;
                }
                break;
            }
        }
    }
    else // SPP
    {
        flag = TRUE;
    }

    return flag;
}

/*******************************************************************************
* Function Name  : bt_hid_data
* Description    : bt_hid_data
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
BOOL BT_TxHidDatas(uint8_t *pdata, uint8_t len, uint8_t type)
{
    BOOL flag = FALSE;
    uint8_t temp0 = ',', temp1 = '\r', temp2 = '\n';
    uint8_t len_lo = (len % 10)+'0', len_hi = (len / 10)+'0';
    
    bt.rx_tout = 0;
    bt.data_ok = 0;
    bt.state = BT_IDLE;

    UART_SendDatas(BT_UART, (uint8_t *)bt_hid_send, strlen(bt_hid_send));
    UART_SendDatas(BT_UART, &type, 1);
    UART_SendDatas(BT_UART, &temp0, 1);
    UART_SendDatas(BT_UART, &len_hi, 1);
    UART_SendDatas(BT_UART, &len_lo, 1);
    UART_SendDatas(BT_UART, &temp0, 1);
    UART_SendDatas(BT_UART, pdata, len);
    UART_SendDatas(BT_UART, &temp1, 1);
    UART_SendDatas(BT_UART, &temp2, 1);

    while (bt.rx_tout < RF_SEND_TOUT)
    {
	  	BSPWatchDogReset();
		
        if (bt.data_ok)
        {
            uint8_t len_tmp = strlen(bt_send_ok);
            
            if ((bt.index_tmp == len_tmp) && \
               (!memcmp(bt.buff_tmp, bt_send_ok, len_tmp)))
            {
                flag = TRUE;
            }
            break;
        }
    }

    return flag;
}



/*用于设置蓝牙名字 add by ckc*/
static bool BT_SetName()
{
	bool flag;
	char BTNameSet[40]={0};
	char BTNameResponse[40]={0};	
	char mac[5];
	
	char len;
	mac[0] = '_';
	mac[1] = bt_mac[9];
	mac[2] = bt_mac[10];
	mac[3] = bt_mac[11];
	mac[4] = '\0';
	
	//拼接设置命令
	sprintf(BTNameSet,"%s","AT+NAME=");
	sprintf(BTNameSet + strlen(BTNameSet),"%s",BARCODE_SCANNER_NAME);

	sprintf(BTNameSet + strlen(BTNameSet),"%s",mac);
	
	sprintf(BTNameSet + strlen(BTNameSet),"%s","\r\n");
	
	
	//拼接响应命令
	sprintf(BTNameResponse,"%s","\r\n+NAME=");
	sprintf(BTNameResponse + strlen(BTNameResponse),"%s",BARCODE_SCANNER_NAME);
	sprintf(BTNameResponse + strlen(BTNameResponse),"%s",mac);
	sprintf(BTNameResponse + strlen(BTNameResponse),"%s","\r\n\r\nOK\r\n");
	
	//获取名字
	flag = BT_Command((uint8_t *)bt_name_get, strlen(bt_name_get), bt.resp_buf, &bt.resp_len);
	//判断名字是否符合要求
	if (flag) // Get BT Name OK
	{
		if (memcmp(bt.resp_buf, BTNameResponse, strlen(BTNameResponse)) || (bt.resp_len != strlen(BTNameResponse))) //不符合重新设置
		{
		#if 1//def _BT_CONF
			flag = BT_Command((uint8_t *)BTNameSet, strlen(BTNameSet), bt.resp_buf, &bt.resp_len);
			if (flag)
			{
				if (memcmp(bt.resp_buf, bt_resp_ok, strlen(bt_resp_ok)))
				{
					flag = FALSE;
				}
			}
		#endif /* _BT_CONF */
		}
	}	
	
	return flag;
}

#endif /* _BT_MODULE */

