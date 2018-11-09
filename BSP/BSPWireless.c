#include <stdlib.h>
#include "MM32F103.h"
#include "Common.h"
#include "BSP.h"
#include "FIFO.h"
#include "DLList.h"
#include "wireless.h"
#include "ConfigAddr.h"

CMD_UPLOAD pktCmdUpload={0xfe,0xef,0,0};	//初始化上传命令包



static Wireless_Type WirelessType = TYPE_2_4G;	//默认2.4G



unsigned char crc_high_first(unsigned char *ptr, unsigned char len)
{
    unsigned char i; 
    unsigned char crc=0x00; /* 计算的初始crc值 */ 

    while(len--)
    {
        crc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */  
        for (i=8; i>0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */  
        { 
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }

    return (crc); 
}


BOOL checkData(unsigned char *ptr, UINT16 len)
{
	uint8_t crc;
	
	crc = crc_high_first(ptr,len-1);
	if(crc == ptr[len-1])
	{
		return TRUE;
	}

	return FALSE;
}



/***************描述：返回当前连接类型****************/
Wireless_Type BSPGetWireleeType(void)
{
	if(BT_LINK_IN) // BT connected
	{
		WirelessType = TYPE_BlueT;
	}
	else
	{
		WirelessType = TYPE_2_4G;
	}
	return WirelessType;
}




/**********2.4G,蓝牙初始化，以及设置回调函数**********/
void BSPWirelessInit(void)
{
	RingBuffer_Init(&uart_ring, uart_buffer, RING_SIZE);
	//添加周期任务
	BSPTimerIntervalCreate(RF_Timer,1);
	BSPTimerIntervalCreate(BT_Timer,1);
	
	rf_init();
	BT_Initial();
//	I435eUartInit();
}




/***********************************
无线通讯发送数据
参数：
	uint8_t* 指针
	uint16_t 长度
返回：
	TRUE	成功
	FALSE	失败
***********************************/
bool BSPWirelessWriteBytes(const uint8_t* Data, uint16_t Count)
{
	__disable_irq();
	if ((Count + 2) <= (uart_ring.buffer_size - RingBuffer_Data_Len(&uart_ring)))	//数据未满
	{
		RingBuffer_Write(&uart_ring, (uint8_t*)&Count, 2);		//写入长度
		RingBuffer_Write(&uart_ring, (uint8_t*)Data, Count);	//写入数据	
	}
	__enable_irq();
	
	return Wireless_Main();
}







/***********************************
描述：	2.4G 收发配对
参数：	void
返回：
	TRUE	成功
	FALSE	失败
***********************************/
bool BSPWirelseeRequestMatch(void)
{
	bool b;
	rf_host.idleout = 0;
	rf_host.rx_init = TRUE;
	
    uart.buff[0] = 8;//-= 2;
    uart.index = uart.buff[0];	

	if ( rf_host.rx_init )
	{
		b = RF_PairForRx();
		if(b)
		{
			GetMAC2_4G();
			BSPEEPROMWrite(CONFIG_2_4MAC, (uint16_t*)GetMAC2_4G(), 2);		
		}
	}	

	BSPWatchDogReset();
	uart.data_ok = 0;	
	return b;
}



bool SendCmd2_4G(CMD_UPLOAD* p);
//char TestString[] = "caoKangChun@@";
char TestString[] = "guangzhousanmakejiyouxiangongsi\r";

//无线通讯循环程序
void BSPWirelessRoutine(void)
{
	pktCmdUpload.sid = getPkt_sid();		//获取sid

	pktCmdUpload.crc = crc_high_first((uint8_t*)&pktCmdUpload,sizeof(CMD_UPLOAD)-1);	//计算crc
	
	SendCmd2_4G(&pktCmdUpload);
//	UART2Routine();
//	
//	if(CONNECT == GetBTStatus())
//	{
//		I435eSendContent(TestString,strlen(TestString));	
//	}
}




//无线通讯设配置
void BSPWirelessConfig(void)
{

}	

//无线通讯打开
bool BSPWirelessOpen(void)
{
	return(TRUE);
}
	
//无线通讯关闭
bool BSPWirelessClose(void)
{
	return(TRUE);
}

//无线通讯复位
bool BSPWirelessReset(void)
{
	return(TRUE);
}

//无线通讯建立连接
bool BSPWirelessConnet(void)
{
	return(TRUE);
}

//无线通讯断开连接
bool BSPWirelessDisconnet(void)
{
	return(TRUE);
}	

//无线通讯在线状态查询
bool BSPWirelessOnline(void)
{
	return(TRUE);
}



//无线通讯查询接收缓冲获得据量
uint16_t BSPWirelessBufCount(void)
{
	return(TRUE);
}

//无线通讯清空接收缓冲区数据
void BSPWirelessBufClear(uint8_t* Data, uint16_t Count)
{
	
}

//无线通讯接收数据一字节
uint8_t BSPWirelessReadByte(void)
{
	return(TRUE);
}	

//无线通讯接收数据多字节
void BSPWirelessReadBytes(uint8_t* Data, uint16_t Count)
{
}

//无线通讯添加发送数据完成中断服务
bool BSPWirelessAddWriteISR(CallBackISR Program)
{
	return(TRUE);
}

//无线通讯添加接收数据完成中断服务
extern bool BSPWirelessAddReadISR(CallBackISR Program)
{
	return(TRUE);
}



