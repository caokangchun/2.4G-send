#include <stdlib.h>
#include "Common.h"
#include "MM32F103.h"


void BSPPowerInit(void) 
{
	GPIO_InitTypeDef GPIOInitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIOInitStructure.GPIO_Pin  =  GPIO_Pin_4;
    GPIOInitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIOInitStructure);
}

void BSPPowerOn(void) 
{
    GPIO_SetBits(GPIOB, GPIO_Pin_4);
}

void BSPPowerOff(void) 
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}
