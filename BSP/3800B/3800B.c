#include "UART1.h"
#include "MM32F103.h"
#include "Common.h"
#include "FIFO.h"
#include "BSP.h"


void CM3800BInit(void)
{
	GPIO_InitTypeDef GPIOInit;
	
	UART1Init(115200);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
    GPIOInit.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_3;
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIOInit);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void CM3800BOpen(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	UART1Open();
}

void CM3800BClose(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	UART1Close();
}

void CM3800BScanStart(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void CM3800BScanStop(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

bool CM3800BAllowRead(void)
{
	return(UART1AllowRead());
}

uint8_t CM3800BRead(void)
{
	return(UART1Read());
}

void CM3800BRoutine(void)
{
	UART1Routine();
}


