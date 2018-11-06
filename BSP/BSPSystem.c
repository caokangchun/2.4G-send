#include <stdlib.h>
#include "Common.h"
#include "MM32F103.h"


//*******************************************************************
//作用：系统初始化
//参数：无
//返回：无
//*******************************************************************
void BSPSystemInit(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);							//开启复用时钟，这个一定要开
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);						//失能JTAG，使能SW
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

//*******************************************************************
//作用：总中断打开
//参数：无
//返回：无
//*******************************************************************
void BSPSystemIntOpen(void)
{
	__enable_irq();
}

//*******************************************************************
//作用：总中断关闭
//参数：无
//返回：无
//*******************************************************************
void BSPSystemIntClose(void)
{
	__disable_irq();
}

//*******************************************************************
//作用：系统复位
//参数：无
//返回：无
//*******************************************************************
void BSPSystemReset(void)
{
	NVIC_SystemReset();
}


