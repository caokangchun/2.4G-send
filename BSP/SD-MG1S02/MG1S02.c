#include <stdlib.h>
#include <string.h>
#include "MM32F103.h"
#include "Common.h"
#include "FIFO.h"
#include "DLList.h"
#include "BSP.h"
#include "UART0.h"
#include "MG1S02.h"


#define		STATE_RECEIVING				0
#define		STATE_EXECUTE_COMMAND		1
#define		STATE_IDLE					2

#define Data_Buffer_Size 2400
#define Len_Buffer_Size 128

typedef struct
{
	uint8_t WorkState;
}
Module_State;

static Module_State ModuleState;

static FIFO DataBuffer;		//数据缓存
static FIFO LenBuffer;		//长度序列

static uint32_t ReceiveTimer = 0;	//接收阻塞计时
static uint8_t RxBuffer[266];		//数据接收缓存
static uint8_t HeadLen = 0;			//头部长度
static uint16_t RxDataLen = 0;		//接收解码数据长度
static uint16_t LenCount = 0;

static uint32_t BlockingTime = 0;	//阻塞时间
static uint16_t AckDataLen = 0; 	//标志位操作返回的数据长度
static uint8_t* FlagBuffer;
static uint8_t CRCBuffer[2];
static uint16_t AckLenCount = 0;

static uint32_t ScanTime = 0;	//扫描时间

//状态寄存字节
//bit6-7	数据接收状态
//bit0-1	回复接收状态
//bit2		发送标志位操作命令及等待回应状态
//bit3		扫描工作状态
static uint8_t StateByte = 0;

void MG1S02Routine(void);
static void MG1S02CheakBaud(void);
static void MG1S02DefaultConfiguration(void);
bool MG1S02SendCommand(uint16_t FlagAddr, uint8_t FlagLen, uint8_t BitsLen, uint8_t BitsOffset, uint16_t Data ,bool NeedToRead);

static void WriteData(void* DataTo, const void* DataFrom)
{
	*((uint8_t*)DataTo) = *((uint8_t*)DataFrom);
}
static void WriteLen(void* DataTo, const void* DataFrom)
{
	*((uint16_t*)DataTo) = *((uint16_t*)DataFrom);
}

//*******************************************************************+
//作用：CRC校验 
//参数：Ptr-校验的字符串
//		Len-校验长度
//返回：无
//*******************************************************************
static uint16_t CRCCalByBit(uint8_t *Ptr, uint8_t Len)
{
	uint32_t crc = 0;
	uint8_t i;

	while(Len-- != 0)
	{
		for(i = 0x80; i != 0; i /= 2)
		{
			crc *= 2;
			if((crc&0x10000) !=0) //上一位 CRC 乘 2 后，若首位是 1，则除以 0x11021
				crc ^= 0x11021;
			if((*Ptr&i) != 0) //如果本位是 1，那么 CRC = 上一位的 CRC + 本位/CRC_CCITT
				crc ^= 0x1021;
		}
		Ptr++;
	}
	return crc;
}

//*******************************************************************
//作用：扫描模块初始化 
//参数：无
//返回：无
//*******************************************************************
void MG1S02Init(void)
{	
	ModuleState.WorkState = STATE_IDLE;
	
	UART0Init(115200);
	UART0Open();
	BSPTimerAddISR(UART0Routine);
	BSPTimerAddISR(MG1S02Routine);
	
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_1);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);
	gpio_bit_set(GPIOB,GPIO_PIN_1); 
	
	MG1S02CheakBaud();
	MG1S02DefaultConfiguration();
	
	//初始化缓冲区
	DataBuffer = FIFOCreate(Data_Buffer_Size, sizeof(uint8_t));
	FIFOCallbackWrite(DataBuffer, WriteData);
	LenBuffer = FIFOCreate(Len_Buffer_Size, sizeof(uint16_t));
	FIFOCallbackWrite(LenBuffer, WriteLen);
	
	ModuleState.WorkState = STATE_RECEIVING;
}

//*******************************************************************
//作用：开始扫描
//参数：无
//返回：无
//*******************************************************************
static void MG1S02ScanStart(void)
{
		ScanTime = 0;
		StateByte |= 0x08;
		gpio_bit_reset(GPIOB,GPIO_PIN_1);	
}

//*******************************************************************
//作用：停止扫描
//参数：无
//返回：无
//*******************************************************************
static void MG1S02ScanStop(void)
{
	ScanTime = 0;
	StateByte &= 0xF7;
	gpio_bit_set(GPIOB,GPIO_PIN_1);
}

//*******************************************************************
//作用：扫描按键响应
//参数：无
//返回：无
//*******************************************************************
void MG1S02Scan(void)
{
	if(gpio_output_bit_get(GPIOB,GPIO_PIN_1) == SET)
	{
		MG1S02ScanStart();
	}
	else
	{
		MG1S02ScanStop();
	}	
}

//*******************************************************************
//作用：接收串口缓存的一个字节
//参数：无
//返回：TRUE-接收完一个解码数据
//      FALSE-接收未完成
//*******************************************************************
static bool MG1S02ReceiveData(void)
{
	uint8_t tmp;
	uint32_t readLen = 0;
	
	uint8_t RxDataState = StateByte>>6;
	
	readLen = UART0GetReadCount();		//串口缓存的数据长度
			
	switch(RxDataState)
	{
		case 0:
		{
			while(readLen--)
			{
				tmp = UART0Read();
				if(tmp == 0x03)		//开始
				{
					RxBuffer[0] = tmp;
					HeadLen = 1;		//已接收头部（协议）长度
					ReceiveTimer = 0;
					RxDataState = 1;	//开始接收
					break;
				}
			}
			break;
		}
			
		case 1:
		{
			while(readLen--)
			{
				tmp = UART0Read();
				RxBuffer[HeadLen++] = tmp;
				if(HeadLen == 3)  //获取前三个字符
				{
					RxDataLen = RxBuffer[1];	//获取解码数据长度
					RxDataLen <<=8;
					RxDataLen |= RxBuffer[2];
					
					if(RxDataLen>(Data_Buffer_Size-FIFOCount(DataBuffer)))
					{
						HeadLen = 0;
						RxDataLen = 0;
						ReceiveTimer = 0;
						StateByte &= 0x3F;
						MG1S02ScanStop();
						return(FALSE);
					}
					
					LenCount = 0;	 //开始计数
					RxDataState = 2; //进入解码数据接收状态
					break;
				}	
			}
			break;
		}
			
		case 2:
		{
			while(readLen--)
			{
				tmp = UART0Read();
				if(LenCount<RxDataLen)	
				{
					FIFOPush(DataBuffer, &tmp); 
					LenCount++;
				}
				if(LenCount>=RxDataLen)	//已获取全部数据
				{
					FIFOPush(LenBuffer,&RxDataLen);
					HeadLen = 0;
					RxDataLen = 0;
					LenCount = 0;
					ReceiveTimer = 0;
					StateByte &= 0x3F;
					MG1S02ScanStop();
					return(TRUE);
				}
			}
			break;
		}
	}
	
	if(RxDataState!=0)
	{
		ReceiveTimer++;
		if(ReceiveTimer>=100)
		{
			if(RxDataState==2)
			{
				FIFOSpecifyClear(DataBuffer, LenCount);
			}
			RxDataState = 0;
		}
	}
	
	StateByte = (StateByte&0x3F) | (RxDataState<<6);
	
	return(FALSE);
}

//*******************************************************************
//作用：判断是否有接受完的数据
//参数：无
//返回：TRUE-可读
//      FALSE-不可读
//*******************************************************************
bool MG1S02AllowRead(void)
{

	if(FIFOCount(LenBuffer))
	{
		return(TRUE);
	}
	
	return(FALSE);
}

//*******************************************************************
//作用：接收标志位操作的回复
//参数：无
//返回：TRUE-操作成功
//      FALSE-操作失败
//*******************************************************************
static bool MG1S02ReceiveAck(void)
{
	uint8_t tmp;
	uint16_t crc; 
	uint32_t readLen = 0;
	uint8_t RxAckState = StateByte&0x03;
	
	while(UART0AllowRead() == TRUE)
	{
		while((StateByte>>6)!=0)
		{
			MG1S02ReceiveData();	//没接收完的先接完
		}
		
		readLen = UART0GetReadCount();		//串口缓存的数据长度
		
		switch(RxAckState)
		{
			case 2:
			{
				while(readLen--)
				{
					tmp = UART0Read();
					RxBuffer[AckLenCount++] = tmp;
					if(AckLenCount >= (AckDataLen+4))
					{
						RxAckState = 3;
						break;
					}
				}
				break;
			}
			
			case 0:
			{
				while(readLen--)
				{
					tmp = UART0Read();
					if(tmp == 0x02)
					{
						RxBuffer[0] = tmp;	
						AckLenCount = 1;
						RxAckState = 1;
						break;
					}
				}
				break;
			}
			
			case 1:
			{
				while(readLen--)
				{
					tmp = UART0Read();
					RxBuffer[AckLenCount++] = tmp;
					if(AckLenCount == 4)
					{
						AckDataLen = tmp;
						if(AckDataLen == 0)
						{
							AckDataLen = 256;
						}
						RxAckState = 2;
						break;
					}
				}
				break;
			}
			
			case 3:
			{
				while(readLen--)
				{
					tmp = UART0Read();
					CRCBuffer[AckLenCount-(AckDataLen+4)] = UART0Read();
					AckLenCount++;
					if(AckLenCount >= (AckDataLen+6))
					{
						crc = CRCCalByBit(&RxBuffer[2],AckDataLen+2);
					
						if(0x00 != RxBuffer[1])		//头错误则返回错误
						{
							return(FALSE);
						}		
						if(0x00 != RxBuffer[2])
						{
							return(FALSE);	
						}
						if(AckDataLen != RxBuffer[3])
						{
							return(FALSE);
						}
						if(CRCBuffer[0] == (uint8_t)(crc>>8) && CRCBuffer[1] == (uint8_t)crc)
						{
							FlagBuffer = &RxBuffer[4];
							return(TRUE);
						}
						return(FALSE);
					}
				}
				break;
			}
			
		}
	}
	
	StateByte = (StateByte&0xFC) | RxAckState;
	
	return(FALSE);
}

//*******************************************************************
//作用：写标志位
//参数：Addr-标志位地址
//		Len-连续写标志位次数（次数为256用0表示）
//		Data-写入标志位的数
//返回：TRUE-写成功
//      FALSE-写失败
//*******************************************************************
static bool MG1S02WriteFlag(uint16_t Addr, uint8_t Len, const uint8_t* Data) 
{
	uint8_t i;
	uint16_t crc;
	uint16_t dataLen;
	uint8_t buf[8+256];
	
	dataLen = Len;
	if(Len == 0)
	{
		dataLen = 256;
	}
	
	buf[0] = 0x7E;	//头
	buf[1] = 0x00;
	
	buf[2] = 0x08;	//写类型
	
	buf[3] = dataLen;		//写数据长度
	
	buf[4] = (uint8_t)(Addr>>8);	//地址
	buf[5] = (uint8_t)Addr;

	for(i=0;i<dataLen;i++)
	{
		buf[6+i] = Data[i];		//数据
	}

	crc = CRCCalByBit(&buf[2],4+dataLen);		//crc校验

	buf[6+dataLen] = (uint8_t)(crc>>8);
	buf[7+dataLen] =(uint8_t)crc;

	for(i=0;i<(8+dataLen);i++)		//8个固定长度+数据长度len
	{
		UART0Write(buf[i]);		//串口发送
	}
	
	StateByte |= 0x04;
	StateByte &= 0xFC;	
	BlockingTime = 0;	//重置阻塞
	
	while(BlockingTime<=(200+dataLen))
	{
		if(MG1S02ReceiveAck() == TRUE)
		{
			StateByte &= 0xFB;
			return(TRUE);
		}
	}
	
	StateByte &= 0xFB;
	return(FALSE);
}

//*******************************************************************
//作用：读标志位 
//参数：Addr-标志位地址
//		Len-连续读标志位的次数
//返回：TRUE-读成功
//      FALSE-读失败
//*******************************************************************
static bool MG1S02ReadFlag(uint16_t Addr, uint8_t Len)
{	
	uint8_t i;
	uint16_t crc;
	uint8_t buf[9];
	
	buf[0] = 0x7E;	//头
	buf[1] = 0x00;
	
	buf[2] = 0x07;	//读类型
	
	buf[3] = 0x01;		
	
	buf[4] = (uint8_t)(Addr>>8);	//地址
	buf[5] = (uint8_t)Addr;

	buf[6] = Len;		//要连续读取的标志位的位数

	crc = CRCCalByBit(&buf[2],5);		//crc校验

	buf[7] = (uint8_t)(crc>>8);
	buf[8] =(uint8_t)crc;

	for(i=0;i<9;i++)			//8个固定长度+数据长度len
	{
		UART0Write(buf[i]);		//串口发送
	}

	StateByte |= 0x04;
	StateByte &= 0xFC;
	BlockingTime = 0;	//重置阻塞
	
	while(BlockingTime<=(200+Len))
	{
		if(MG1S02ReceiveAck() == TRUE)
		{
			StateByte &= 0xFB;
			return(TRUE);
		}
	}
	
	StateByte &= 0xFB;
	return(FALSE);
}

//*******************************************************************
//作用：校对模块与串口的波特率 
//参数：无
//返回：无
//*******************************************************************
static void MG1S02CheakBaud(void)	
{
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	uint16_t value;
	uint8_t baudFlag = 0;
	
	while(1)
	{
		UART0Close();
		switch(baudFlag)
		{
			case 0:
			{
				UART0SetBaud(115200);
				break;
			}
			case 1:
			{
				UART0SetBaud(9600);
				break;
			}
			case 2:
			{
				UART0SetBaud(57600);
				break;
			}
			case 3:
			{
				UART0SetBaud(38400);
				break;
			}
			case 4:
			{
				UART0SetBaud(19200);
				break;
			}
			case 5:
			{
				UART0SetBaud(14400);
				break;
			}
			case 6:
			{
				UART0SetBaud(4800);
				break;
			}
			case 7:
			{
				UART0SetBaud(1200);
				break;
			}
		}
		UART0Open();
		
		if(MG1S02ReadFlag(0x002A,2) == TRUE)
		{
			value = ((FlagBuffer[1]<<8) | FlagBuffer[0]) & 0x1FFF;	//取bit12-0
			switch(value)
			{
				case 0x001A:
				{
					if(baudFlag == 0)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x0139:
				{
					if(baudFlag == 1)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x0034:
				{
					if(baudFlag == 2)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x004E:
				{
					if(baudFlag == 3)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x009C:
				{
					if(baudFlag == 4)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x00D0:
				{
					if(baudFlag == 5)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x0271:
				{
					if(baudFlag == 6)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}
				case 0x09C4:
				{
					if(baudFlag == 7)
					{
						ModuleState.WorkState = STATE_RECEIVING;
						return ;
					}
					break;
				}

			}
		}
		
		baudFlag++;
		if(baudFlag>=8)
		{
			baudFlag = 0;
		}
	}
}

//*******************************************************************
//作用：强制设置指定功能 
//参数：无
//返回：无
//*******************************************************************
static void MG1S02DefaultConfiguration(void)
{
	uint8_t state = 0;
	uint8_t data;
	uint8_t tmp;
	
	while(1)
	{
		switch(state)
		{
			case 0:
			{
				if(MG1S02ReadFlag(0x000D,1) == TRUE)	//读输出UTF8、串口输出
				{
					data = FlagBuffer[0];
					tmp = data<<4;
					if(0xC0 == tmp)
					{
						state = 2;
					}
					else
					{
						data &= 0xF0;
						data |= 0x0C;
						state = 1;
					}		
				}
				break;
			}
			case 1:
			{
				if(MG1S02WriteFlag(0x000D,1,&data) == TRUE)	//写输出UTF8、串口输出
				{
					state = 2;
				}
				break;
			}
			case 2:
			{
				if(MG1S02ReadFlag(0x0060,1) == TRUE)	//读串口带协议
				{
					data = FlagBuffer[0];
					if((data>>7) == 1)
					{
						return;
					}
					else
					{
						data |= 0x80;
						state = 3;
					}
				}
				break;
			}
			case 3:
			{
				if(MG1S02WriteFlag(0x0060,1,&data) == TRUE)	//写串口带协议
				{
					return;
				}
				break;
			}
		}
	}
}


//*******************************************************************
//作用：唤醒摄像头（串口中断唤醒）
//参数：无
//返回：无
//*******************************************************************
void MG1S02Open(void)	//唤醒
{
	uint8_t i;
	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	uint8_t buf[9]={0x7E,0x00,0x08,0x01,0x00,0xD9,0x00,0xDB,0x26};

	for(i=0;i<9;i++)
	{
		UART0Write(buf[i]);		//串口发送
	}
	
	StateByte |= 0x04;	
	BlockingTime = 0;	//重置阻塞
	
	while(BlockingTime<=50)
	{
		
	}
	
	StateByte &= 0xFB;;
	ModuleState.WorkState = STATE_RECEIVING;
}

//*******************************************************************
//作用：关闭摄像头（深度睡眠1.8mA）
//参数：无
//返回：无
//*******************************************************************
void MG1S02Close(void)	
{
	uint8_t i;
	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	uint8_t buf[9]={0x7E,0x00,0x08,0x01,0x00,0xD9,0xA5,0x3E,0x69};

	for(i=0;i<9;i++)
	{
		UART0Write(buf[i]);		//串口发送
	}
	
	StateByte |= 0x04;	
	BlockingTime = 0;	//重置阻塞
	
	while(BlockingTime<=50)
	{
		
	}
	
	StateByte &= 0xFB;
	ModuleState.WorkState = STATE_RECEIVING;
}

//*******************************************************************
//作用：保存标志位到flash 
//参数：无
//返回：TRUE-保存成功
//      FALSE-保存失败
//*******************************************************************
bool MG1S02SaveFlag(void)
{
	uint8_t i;
	uint8_t buf[9];
	
	buf[0] = 0x7E;		//头
	buf[1] = 0x00;	
	
	buf[2] = 0x09;		//类型
	
	buf[3] = 0x01;		//长度
	
	buf[4] = 0x00;		//地址
	buf[5] = 0x00;

	buf[6] = 0x00;		//datas

	buf[7] = 0xDE;		//crc校验
	buf[8] = 0xC8;

	for(i=0;i<9;i++)			//8个固定长度+数据长度len
	{
		UART0Write(buf[i]);		//串口发送
	}
	
	StateByte |= 0x04;
	StateByte &= 0xFC;
	BlockingTime = 0;	//重置阻塞
	
	while(BlockingTime<=100)
	{
		if(MG1S02ReceiveAck() == TRUE)
		{
			StateByte &= 0xFB;
			return(TRUE);
		}
	}
	
	StateByte &= 0xFB;
	return(FALSE);
}

//*******************************************************************
//作用：标志位恢复到出厂设置
//参数：无
//返回：TRUE-恢复成功
//      FALSE-恢复失败
//*******************************************************************
bool MG1S02FlagFactorySetting(void) 
{
	uint8_t i;
	uint16_t crc;
	uint8_t buf[9];
	
	buf[0] = 0x7e;		//头
	buf[1] = 0x00;	
	
	buf[2] = 0x09;		//类型
	
	buf[3] = 0x01;		//写数据长度
	
	buf[4] = 0x00;		//地址
	buf[5] = 0x00;

	buf[6] = 0xFF;		//datas

	crc = CRCCalByBit(&buf[2],5);		//crc校验

	buf[7] = (uint8_t)(crc>>8);
	buf[8] = (uint8_t)crc;

	for(i=0;i<9;i++)			//8个固定长度+数据长度len
	{
		UART0Write(buf[i]);		//串口发送
	}
	
	StateByte |= 0x04;
	StateByte &= 0xFC;
	BlockingTime = 0;	//重置阻塞
	
	while(BlockingTime<=100)
	{
		if(MG1S02ReceiveAck() == TRUE)
		{
			StateByte &= 0xFB;
			return(TRUE);
		}
	}
	
	StateByte &= 0xFB;
	return(FALSE);
}

//*******************************************************************
//作用：设置波特率
//参数：Baud-波特率
//返回：TRUE-设置成功
//      FALSE-设置失败
//*******************************************************************
bool MG1S02SetBaud(uint32_t Baud)
{
	uint8_t data[2];
	uint16_t val;
	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	switch (Baud)
	{
		case 1200:
		{
			val=0x09c4;
			break;
		}			
		case 4800:
		{
			val=0x0271;
			break;
		}		
		case 9600:
		{
			val=0x0139;
			break;
		}		
		case 14400:
		{
			val=0x00d0;
			break;	
		}	
		case 19200:
		{
			val=0x009c;
			break;
		}		
		case 38400:
		{
			val=0x004e;
			break;
		}
		case 57600:
		{
			val=0x0034;
			break;	
		}			
		case 115200:
		{
			val=0x001a;
			break;
		}		
		default: val=0x001a;	//115200
	}
	data[0] = val;
	data[1] = val>>8;
	
	if(MG1S02WriteFlag(0x002A,2,data) == TRUE)
	{
		UART0SetBaud(Baud);	//当写成功后，需要重新初始化串口
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}

//*******************************************************************
//作用：获得波特率
//参数: Baud-波特率的值的指针
//返回：TRUE-获取成功
//      FALSE-获取失败
//*******************************************************************
bool MG1S02GetBaud(uint32_t* Baud)
{	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	*Baud = 0;
	uint16_t buf = 0;
	
	if(MG1S02ReadFlag(0x002A,2) == TRUE)
	{
		buf = FlagBuffer[1];
		buf = buf<<8 | FlagBuffer[0];
		
		switch (buf)
		{
			case 0x09c4:
			{
				*Baud=1200;
				break;
			}			
			case 0x0271:
			{	
				*Baud=4800;
				break;
			}		
			case 0x0139:
			{
				*Baud=9600;
				break;
			}		
			case 0x00d0:
			{
				*Baud=14400;
				break;	
			}	
			case 0x009c:
			{
				*Baud=19200;
				break;
			}		
			case 0x004e:
			{
				*Baud=38400;
				break;
			}
			case 0x0034:
			{
				*Baud=57600;
				break;	
			}			
			case 0x001a:
			{
				*Baud=115200;
				break;
			}		
			default:
			{
				ModuleState.WorkState = STATE_RECEIVING;
				return(FALSE);
			}
		}
		
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}

//*******************************************************************
//作用：设置字符值
//参数:	Str-用来修改的字符串
//		Addr-字符地址
//		Len-要设置的字符长度
//返回：TRUE-修改成功
//      FALSE-修改失败
//*******************************************************************
bool MG1S02SetCharacterValue(uint32_t Addr, const char *Str, uint16_t Len)
{	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	if(Len > 15)
	{
		Len = 15;
	}

	if(MG1S02WriteFlag(Addr,Len,(uint8_t*)Str) == TRUE)		//修改字符
	{
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}

//*******************************************************************
//作用：获得字符值
//参数:	Str-获得的字符串的值指针
//		Addr-字符地址
//		Len-要读的字符长度
//返回：TRUE-读成功
//      FALSE-读失败
//*******************************************************************
bool MG1S02GetCharacterValue(uint32_t Addr, char *Str, uint16_t Len)
{	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	uint8_t i;
	
	if(Len > 15)
	{
		Len = 15;
	}

	if(MG1S02ReadFlag(Addr,Len) == TRUE)
	{
		for(i=0;i<Len;i++)
		{
			Str[i] = FlagBuffer[i];
		}
		
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}


//*******************************************************************
//作用：读取版本号
//参数:	Addr-硬件版本或软件版本地址
//		Version-版本号字符串指针
//返回：TRUE-读成功
//      FALSE-读失败
//*******************************************************************
bool MG1S02GetVersion(uint32_t Addr, char* Version)
{	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	uint8_t tmp;
	
	if(MG1S02ReadFlag(Addr,1) == TRUE)
	{
		tmp = FlagBuffer[0];
		Version[0] = 'V';
		Version[1] = tmp/100+0x30;
		Version[2] = '.';
		Version[3] = (tmp/10)%10+0x30;
		Version[4] = tmp%10+0x30;
		
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}


//*******************************************************************
//作用：读取软件日期
//参数:	Date-软件日期字符串指针
//返回：TRUE-读成功
//      FALSE-读失败
//*******************************************************************
bool MG1S02GetSoftwareDate(char* Date)
{	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	if(MG1S02ReadFlag(0x00E3,3) == TRUE)
	{
		//year
		Date[0] = 0x32;
		Date[1] = FlagBuffer[0]/100+0x30;
		Date[2] = (FlagBuffer[0]/10)%10+0x30;
		Date[3] = FlagBuffer[0]%10+0x30;
		Date[4] = '.';
		
		//month
		Date[5] = FlagBuffer[1]/10+0x30;
		Date[6] = FlagBuffer[1]%10+0x30;
		Date[7] = '.';
		
		//day
		Date[8] = FlagBuffer[2]/10+0x30;
		Date[9] = FlagBuffer[2]%10+0x30;
		
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}

//*******************************************************************
//作用：读取产品型号
//参数:	Model-产品型号字符串指针
//返回：TRUE-读成功
//      FALSE-读失败
//*******************************************************************
bool MG1S02GetProductModel(char* Model)
{	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	if(MG1S02ReadFlag(0x00E0,3) == TRUE)
	{
		if(FlagBuffer[0] == 0x02)
		{
			strcpy(Model,"SD_MG1S02");
		}	
		else if(FlagBuffer[0] == 0x03)
		{
			strcpy(Model,"SD_MG1S03");
		}
		
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}


//*******************************************************************
//作用：发送串口命令
//参数: FlagAddr-相关的标志位地址
//		FlagLen-连续修改的标志位长度
//		BitsLen-指定设置所占字节的位长度
//		BitsOffset-指定设置所在的字节位偏移
//		Data-用于修改的数据
//		NeedToRead-是否需要先读
//返回：TRUE-设置成功
//      FALSE-设置失败
//*******************************************************************
bool MG1S02SendCommand(uint16_t FlagAddr, uint8_t FlagLen, uint8_t BitsLen, uint8_t BitsOffset, uint16_t Data ,bool NeedToRead)
{
	uint8_t value[2];	//理论上可以设定256字节，但目前串口命名的标志位最大不超过16位，所以2字节足够
	uint8_t state = 0;
	uint16_t i;
	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	while(state!=3)
	{
		switch(state)
		{
			case 0:
			{
				if(NeedToRead == TRUE)
				{
					state = 1;
				}
				else
				{
					value[0] = Data;
					value[1] = Data>>8;
					state =  2;
				}
				break;
			}
			
			case 1:
			{
				if(MG1S02ReadFlag(FlagAddr,FlagLen) == TRUE)
				{
					if(FlagLen == 2)
					{
						uint16_t buf = 0;
						uint16_t tmp = 0;
						buf = FlagBuffer[0]|(FlagBuffer[1]<<8);
						for(i=0;i<FlagLen;i++)
						{
							tmp |= (1<<(i+BitsOffset));
						}
						Data = (Data<<BitsOffset)&tmp;
						buf &= ~tmp;
						buf |= Data;
						value[0] = buf>>8;
						value[1] = buf;
					}
					else
					{
						uint8_t tmp = 0;
						value[0] = FlagBuffer[0];
						for(i=0;i<BitsLen;i++)
						{
							tmp |= (1<<(i+BitsOffset));
						}
						Data = (Data<<BitsOffset)&tmp;
						value[0] &= ~tmp;
						value[0] |= Data;
					}
					state = 2;
				}
				else
				{
					state = 3;
				}
				break;
			}
			
			case 2:
			{
				if(MG1S02WriteFlag(FlagAddr,FlagLen,value) == TRUE)
				{
					ModuleState.WorkState = STATE_RECEIVING;
					return(TRUE);
				}
				else
				{
					state = 3;
				}
				break;
			}
			
			case 3:
			{
				break;
			}
		}
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}

//*******************************************************************
//作用：读取设置
//参数: FlagAddr-相关的标志位地址
//		FlagLen-连续修改的标志位长度
//		BitsLen-指定设置所占字节的位长度
//		BitsOffset-指定设置所在的字节位偏移
//		Value-设置在标志位中的值
//返回：TRUE-获取成功
//      FALSE-获取失败
//*******************************************************************
bool MG1S02GetSetting(uint16_t FlagAddr, uint8_t FlagLen, uint8_t BitsLen, uint8_t BitsOffset, uint16_t* Value)
{
	*Value = 0;
	
	ModuleState.WorkState = STATE_EXECUTE_COMMAND;
	
	if(MG1S02ReadFlag(FlagAddr,FlagLen) == TRUE)
	{
		if(FlagLen == 2)
		{
			uint16_t buf = 0;
			buf = (FlagBuffer[0]<<8) | FlagBuffer[1];
			buf <<= 16 - (BitsOffset+BitsLen);
			buf >>= 16 - BitsLen;
			*Value = buf;
		}
		else
		{
			uint8_t buf = 0;
			buf = FlagBuffer[0];
			buf <<= 8 - (BitsOffset+BitsLen);
			buf >>= 8 - BitsLen;
			*Value = buf;
		}
		
		ModuleState.WorkState = STATE_RECEIVING;
		return(TRUE);
	}
	
	ModuleState.WorkState = STATE_RECEIVING;
	return(FALSE);
}

//*******************************************************************
//作用：读取一个已接收完成的扫码数据，并解析
//参数: Data-提供其他模块引用数据的指针
//		DataLen-提供其他模块引用的数据长度指针
//返回：	无
//*******************************************************************
void MG1S02ReadData(uint8_t* Data, uint16_t* DataLen)
{
	uint16_t len;
	uint16_t i;
	
	FIFOPop(LenBuffer,&len);
	*DataLen = len;
	
	for(i=0;i<len;i++)
	{
		FIFOPop(DataBuffer,&Data[i]);
	}
}

//*******************************************************************
//作用：模块的周期任务（需要放在1毫秒的定时器执行）
//参数: 无
//返回：无
//*******************************************************************
void MG1S02Routine(void)
{
	if((StateByte>>3)&1)
	{
		ScanTime++;
		if(ScanTime > 1000)
		{
			ScanTime = 0;
			StateByte &= 0xF7;
			gpio_bit_set(GPIOB,GPIO_PIN_1);	
		}
	}
	
	switch(ModuleState.WorkState)
	{ 
		case STATE_RECEIVING:
		{
			while(UART0AllowRead() == TRUE)
			{
				MG1S02ReceiveData();
			}
			break;
		}
		
		case STATE_EXECUTE_COMMAND:
		{
			MG1S02ScanStop();
			if((StateByte>>2)&1)
			{
				BlockingTime++;
			}
			break;
		}
		
		case STATE_IDLE:
		{
			break;
		}
	}
}


