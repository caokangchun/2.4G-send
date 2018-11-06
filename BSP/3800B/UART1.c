#include <stdlib.h>
#include "MM32F103.h"
#include "Common.h"
#include "FIFO.h"
#include "BSP.h"


#define CALL_BACK_MAX			8
#define UART_WRITE_WAITTING		1


static void Null(void);


static FIFO Read;
static FIFO Write;
static uint16_t WriteDly = 0;
static CallBackISR ReadISR[CALL_BACK_MAX] = 
{
	Null,
	Null,
	Null,
	Null,
	Null,
	Null,
	Null,
	Null
};
static CallBackISR WriteISR[CALL_BACK_MAX] = 
{
	Null,
	Null,
	Null,
	Null,
	Null,
	Null,
	Null,
	Null
};


static void WriteData(void* DataTo, const void* DataFrom)
{
	*((uint8_t*)DataTo) = *((uint8_t*)DataFrom);
}

static void Null(void)
{
}

//*******************************************************************
//作用：UART初始化
//参数：BPS-波特率
//返回：无
//*******************************************************************
void UART1Init(uint32_t BPS)
{
	GPIO_InitTypeDef GPIOInit;
    UART_InitTypeDef USARTInit;
    NVIC_InitTypeDef NVICInit; 
	
	//初始化USART2中断优先级
	NVICInit.NVIC_IRQChannel = UART1_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInit.NVIC_IRQChannelSubPriority = 0;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICInit);

    //初始化UART1管脚	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
    GPIOInit.GPIO_Pin = GPIO_Pin_9;
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOInit);
    GPIOInit.GPIO_Pin = GPIO_Pin_10;
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIOInit);
	
	//初始化UART1参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
	USARTInit.UART_BaudRate = BPS;
	USARTInit.UART_WordLength = UART_WordLength_8b;
	USARTInit.UART_StopBits = UART_StopBits_1;
	USARTInit.UART_Parity = UART_Parity_No;
	USARTInit.UART_Mode = UART_Mode_Tx | UART_Mode_Rx;
	USARTInit.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
	UART_Init(UART1, &USARTInit);
	UART_ITConfig(UART1, UART_IT_TXIEN, ENABLE);
	UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);

    //初始化缓冲区
	Read = FIFOCreate(128, sizeof(uint8_t));
	FIFOCallbackWrite(Read, WriteData);
	Write = FIFOCreate(32, sizeof(uint8_t));
	FIFOCallbackWrite(Write, WriteData);
}

//*******************************************************************
//作用：UART添加读中断程序
//参数：Function-添加当定时器中断时执行的程序，最多添加8个
//返回：TRUE-成功
//      FALSE-失败 
//*******************************************************************
bool UART1AddReadISR(CallBackISR Function)
{
	static uint8_t count = 0;

    if(count < CALL_BACK_MAX)
    {
        ReadISR[count] = Function;
        ++count;
        return(TRUE);
    }
	return(FALSE);
}

//*******************************************************************
//作用：UART添加写中断程序
//参数：Function-添加当定时器中断时执行的程序，最多添加8个
//返回：TRUE-成功
//      FALSE-失败 
//*******************************************************************
bool UART1AddWriteISR(CallBackISR Function)
{
	static uint8_t count = 0;

    if(count < CALL_BACK_MAX)
    {
        WriteISR[count] = Function;
        ++count;
        return(TRUE);
    }
	return(FALSE);
}

//*******************************************************************
//作用：UART打开
//参数：无
//返回：无
//*******************************************************************
void UART1Open(void)
{
	UART_Cmd(UART1, ENABLE);
}

//*******************************************************************
//作用：UART关
//参数：无
//返回：无
//*******************************************************************
void UART1Close(void)
{
	FIFOClear(Read);
	FIFOClear(Write);
	UART_Cmd(UART1, DISABLE);
}

//*******************************************************************
//作用：UART写数据
//参数：Data-发送的数据
//返回：无
//*******************************************************************
void UART1Write(uint8_t Data)
{
	uint8_t tmp = Data;
	
	__disable_irq();
	if(WriteDly)
	{
	    __enable_irq();
		FIFOPush(Write, &tmp);			
	}
	else
	{
		if(UART_GetFlagStatus(UART1, UART_FLAG_TXEPT) == SET)
		{	
			UART_SendData(UART1, Data);
			__enable_irq();
			return;
		}
		__enable_irq();
		FIFOPush(Write, &tmp);	
	}
}

//*******************************************************************
//作用：查询UART是否有数据可以读取
//参数：无
//返回：TRUE-有数据可读
//      FALSE-无数据可读
//*******************************************************************
bool UART1AllowRead(void)
{
	if(FIFOCount(Read))
	{
		return(TRUE);
	}
	return(FALSE);
}

//*******************************************************************
//作用：UART读数据
//参数：无
//返回：接收的数据
//*******************************************************************
uint8_t UART1Read(void)
{
	uint8_t tmp;
	
	FIFOPop(Read, &tmp);	
	return(tmp);
}

//*******************************************************************
//作用：UART清空读缓冲区
//参数：无
//返回：无
//*******************************************************************
void UART1BufClear(void)
{
	FIFOClear(Read);
	FIFOClear(Write);
}

//*******************************************************************
//作用：UART周期性任务
//参数：无
//返回：无
//*******************************************************************
void UART1Routine(void)
{
    uint8_t tmp;

	__disable_irq();
	if(WriteDly)
	{
		--WriteDly;
		__enable_irq();
	}
	else
	{
		if(FIFOCount(Write))
		{
			if(UART_GetFlagStatus(UART1, UART_FLAG_TXEPT) == SET)
			{
				FIFOPop(Write, &tmp);
				UART_SendData(UART1, tmp);
			}
		}
		__enable_irq();
	}
}

void UART1_IRQHandler(void)
{
	uint8_t tmp;
	uint8_t i = 0;
	
	if(UART_GetITStatus(UART1, UART_IT_TXIEN) == SET)
    {
		UART_ClearITPendingBit(UART1, UART_IT_TXIEN);
    	if(FIFOCount(Write))
    	{
	    	FIFOPop(Write, &tmp);
			UART_SendData(UART1, tmp);
	    }

    	while(i < CALL_BACK_MAX)
    	{
    		WriteISR[i]();
    		++i;
    	}
    }
	else if(UART_GetITStatus(UART1, UART_IT_RXIEN) == SET)
    {
		UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
		tmp = UART_ReceiveData(UART1);
    	FIFOPush(Read, &tmp);
		WriteDly = UART_WRITE_WAITTING;

    	while(i < CALL_BACK_MAX)
    	{
    		ReadISR[i]();
    		++i;
    	}
    }
    return;
}





