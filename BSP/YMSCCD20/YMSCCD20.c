#include "UART1.h"
#include "MM32F103.h"
#include "Common.h"
#include "FIFO.h"
#include "BSP.h"


void YMSCCD20Init(void)
{
	GPIO_InitTypeDef GPIOInit;
	
	UART1Init(115200);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
    GPIOInit.GPIO_Pin = GPIO_Pin_15;
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIOInit);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void YMSCCD20Open(void)
{
	UART1Open();
}

void YMSCCD20ScanStart(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

void YMSCCD20ScanStop(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

bool YMSCCD20AllowRead(void)
{
	return(UART1AllowRead());
}

uint8_t YMSCCD20Read(void)
{
	return(UART1Read());
}

void YMSCCD20Routine(void)
{
	UART1Routine();
}


