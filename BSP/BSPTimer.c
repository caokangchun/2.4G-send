#include "MM32F103.h"
#include "BSP.h"


#define CALL_BACK_MAX					10

#define	SYSTICK_TIMER_TYPE_NULL			0
#define	SYSTICK_TIMER_TYPE_INTERVAL		1
#define	SYSTICK_TIMER_TYPE_ONCE			2


typedef struct
{
	CallBackISR ISR;
	uint16_t Scale;
	uint16_t Tick;
	uint8_t Type;
}
CallBack_Program;


static void Null(void);


static CallBack_Program Program[CALL_BACK_MAX] = 
{
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL},
	{Null, 0, 0, SYSTICK_TIMER_TYPE_NULL}
};


static void Null(void)
{
}

//*******************************************************************
//作用：系统时基初始化，内部固定1ms中断一次
//参数：无
//返回：TRUE-成功
//      FALSE-失败 
//*******************************************************************
bool BSPTimerInit(void)
{	
	if(SysTick_Config(1000 * ((SystemCoreClock / 8) / 1000000)) == 1)
	{
		return(FALSE);
	}	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	return(TRUE);
}

//*******************************************************************
//作用：创建间隔执行的定时器，误差+-1ms
//参数：Function-添加当定时器中断时执行的程序
//		Scale-定时比例，代表程序执行的时间隔间，单位1ms
//返回：1至10-定时器编号
//      0-失败 
//*******************************************************************
uint8_t BSPTimerIntervalCreate(CallBackISR Function, uint16_t Scale)
{
	uint8_t count = 0;
	
    while(count < CALL_BACK_MAX)
    {
		if(Program[count].Type == SYSTICK_TIMER_TYPE_NULL)
		{
			Program[count].ISR = Function;
			Program[count].Scale = Scale;
			Program[count].Tick = Scale;
			Program[count].Type = SYSTICK_TIMER_TYPE_INTERVAL;
			return(count + 1);
		}
        ++count;
    }
	return(0);
}

//*******************************************************************
//作用：删除间隔执行的定时器
//参数：TimerNum-定时器编号
//返回：无
//*******************************************************************
void BSPTimerIntervalDestroy(uint8_t TimerNum)
{
	--TimerNum;
	BSPSystemIntClose();
	Program[TimerNum].ISR = Null;
	Program[TimerNum].Scale = 0;
	Program[TimerNum].Tick = 0;
	Program[TimerNum].Type = SYSTICK_TIMER_TYPE_NULL;
	BSPSystemIntOpen();
}

//*******************************************************************
//作用：创建执行一次的定时器，误差+-1ms
//参数：Function-添加当定时器中断时执行的程序
//		Scale-定时比例，代表程序执行的等待时间，单位1ms
//返回：TRUE-成功
//      FALSE-失败 
//*******************************************************************
bool BSPTimerOnceCreate(CallBackISR Function, uint16_t Scale)
{
	uint8_t count = 0;
	
    while(count < CALL_BACK_MAX)
    {
		if(Program[count].Type == SYSTICK_TIMER_TYPE_NULL)
		{
			Program[count].ISR = Function;
			Program[count].Scale = Scale;
			Program[count].Tick = Scale;
			Program[count].Type = SYSTICK_TIMER_TYPE_ONCE;
			return(TRUE);
		}
        ++count;
    }
	return(FALSE);
}

//*******************************************************************
//作用：等待方式延时
//参数：Scale-延时比例，代表延时时间，最大值500，最小值50，单位1us
//返回：无
//*******************************************************************
void BSPTimerDelay(uint16_t Scale)
{
	uint32_t currentT;
	uint32_t scaleT;
	uint32_t startT = SysTick->VAL;
	uint32_t tmp;
	
	if(Scale > 500)
	{
		Scale = 500;
	}
	else if(Scale < 50)
	{
		Scale = 50;
	}
	
	scaleT = (uint32_t)Scale * 10000;
	scaleT /= (0.083333333 * 10000);
	
	if(startT > scaleT)
	{
		while(1)
		{
			currentT = SysTick->VAL;
			if(startT - currentT > scaleT)
			{
				return;
			}
		}
	}
	else
	{
		tmp = (12000 - 1) - (scaleT - startT);
		while(1)
		{
			currentT = SysTick->VAL;
			if(currentT > startT && currentT < tmp)
			{
				return;
			}
		}
	}
}

void SysTick_Handler(void)
{
	uint8_t i = 0;
	
	while(i < CALL_BACK_MAX)
	{
		if(Program[i].Type == SYSTICK_TIMER_TYPE_INTERVAL)
		{
			if(--Program[i].Tick == 0)
			{
				Program[i].ISR();
				Program[i].Tick = Program[i].Scale;
			}
		}
		else if(Program[i].Type == SYSTICK_TIMER_TYPE_ONCE)
		{
			if(--Program[i].Tick == 0)
			{
				Program[i].ISR();
				Program[i].ISR = Null;
				Program[i].Scale = 0;
				Program[i].Tick = 0;
				Program[i].Type = SYSTICK_TIMER_TYPE_NULL;
			}
		}
		++i;
	}
}

