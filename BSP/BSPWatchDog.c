#include "MM32F103.h"
#include "Common.h"
#include "BSPWatchDog.h"




static void PVU_CheckStatus(void);
static void RVU_CheckStatus(void);


/*******************************************************************
作用：看门狗初始化复位时间，并开始运行
参数：
	MS409,
	MS819,
	MS1638,
	MS3276,
	MS6553,
	MS13107,
	MS26214,
返回：无
*******************************************************************/
extern void BSPWatchDogInit(WDTInterval time)
{
    /*启动内部低速时钟,等待时钟就绪*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    
    /*设置时钟预分频*/	
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
	switch(time)
	{
		case MS409:
			IWDG_SetPrescaler(IWDG_Prescaler_4);
			break;
		
		case MS819:
			IWDG_SetPrescaler(IWDG_Prescaler_8);
			break;

		case MS1638:
			IWDG_SetPrescaler(IWDG_Prescaler_16);
			break;
		
		case MS3276:
			IWDG_SetPrescaler(IWDG_Prescaler_32);
			break;
		
		case MS6553:
			IWDG_SetPrescaler(IWDG_Prescaler_64);
			break;		
		
		case MS13107:
			IWDG_SetPrescaler(IWDG_Prescaler_128);
			break;
		
		case MS26214:
			IWDG_SetPrescaler(IWDG_Prescaler_256);
			break;
		
		default: break;
	}
    
    
    /*设置重载寄存器值*/	
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(0xfff);	
    
    /*装载并使能计数器*/	
    IWDG_ReloadCounter();
    IWDG_Enable();	
}


/*******************************************************************
作用：喂狗函数
参数：无
返回：无
*******************************************************************/
void BSPWatchDogReset(void)
{
	IWDG_ReloadCounter();
}




/********************************************************************************************************
**函数信息 ：PVU_CheckStatus(void)                       
**功能描述 ：检查独立看门狗预分频位状态
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
static void PVU_CheckStatus(void)
{
    while(1)
    {
        /*检查预分频位状态,为RESET才可改变预分频值*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)                                                       
        {
            break;
        }
    }
}
/********************************************************************************************************
**函数信息 ：RVU_CheckStatus(void)                  
**功能描述 ：检查独立看门狗重载标志
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
static void RVU_CheckStatus(void)
{
    while(1)
    {
        /*检查重载标志状态*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  
        {
            break;
        }
    }
}
