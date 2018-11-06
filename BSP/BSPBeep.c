#include <stdlib.h>
#include "Common.h"
#include "gd32f1x0.h"
#include "BSPBeep.h"
#include "ActionConfig.h"
#include "BSP.h"
	
#define CALL_BACK_MAX					8
	
#define TIME1_PSC						71U					//设置用来作为TIMx时钟频率除数的预分频值
#define TIME0_PSC						71U					//设置用来作为TIMx时钟频率除数的预分频值

#define TIM_CLK  						(72U * 1000000U)	//时钟源

	
static void BeepGpioConfig(void);				//蜂鸣器引脚配置.
static void BeepTimeSet(uint32_t t);			//蜂鸣器时间设置.
static void Timer1PwmInit(void);				//定时器1 PWM输出初始化.
static void Time0Init(uint32_t Interval);		//定时器0 初始化.
static void BeepPlayRoutine(void);				//蜂鸣器播放任务.
static void BSPTime0Routine(void);				//定时器0周期任务
	
static uint32_t BeepTime = 0;					//蜂鸣时间
static uint32_t ReloadValue = 0;				//重加载值
static uint8_t	DutyCycle = 50;					//占空比
static uint32_t BeepCountTime=0;				//计数值


uint16_t Tone2[] = {0,1361,1412,1527,1712,1770,2066};

uint8_t Song_NewSetBeep[]={2,4,6,16};
uint8_t Beat_NewSetBeep[]={50,50,100};

uint8_t Song_NewErrorBeep[]={5,3,1,16};
uint8_t Beat_NewErrorBeep[]={100,100,150};

uint8_t Song_NewSetErrorBeep[]={2,4,6,5,3,1,16};
uint8_t Beat_NewSetErrorBeep[]={50,50,100,100,100,150};

uint8_t Song_NewStartBeep[]={6,0,6,16};
uint8_t Beat_NewStartBeep[]={92,50,82};

static void Null(void)
{
}

static CallBackISR ISR[CALL_BACK_MAX] = 
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

/***************************************************
作用：	蜂鸣器初始化.
参数：	无
返回:	无
***************************************************/
void BSPBeepInit(void)
{
	
	BeepGpioConfig();
	Timer1PwmInit();

	Time0Init(1000);
	
	BSPTimer0AddFunc(BSPTime0Routine);
}

/***************************************************
作用：	定时器0周期任务.
参数：	无
返回:	无
***************************************************/
void BSPTime0Routine(void)
{
	BeepPlayRoutine();
	BSPLEDFlashRoutine();
	
}

/***************************************************
作用：	蜂鸣器引脚配置.
参数：	无
返回:	无
***************************************************/
static void BeepGpioConfig(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	
	gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_3);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_3);
	
	gpio_af_set(GPIOB,GPIO_AF_2,GPIO_PIN_3);//引脚复用
}


/***************************************************
作用：	定时器1 PWM输出初始化.
参数：	无
返回:	无
***************************************************/
static void Timer1PwmInit(void)
{
	timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);
	timer_deinit(TIMER1);

    /* TIMER1 configuration */
    timer_initpara.prescaler         = TIME1_PSC;//72M/(71+1)=1MHz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;//1ms
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);
	
	/* CH1 configuration in OC active mode */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	
	timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);
	
//	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 500);//配置捕获值
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
	
	 /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    timer_enable(TIMER1);

}

/***************************************************
作用：	定时器0 初始化.
参数：	Interval-定时器中断间隔时间，单位us
返回:	无
***************************************************/
static void Time0Init(uint32_t Interval)
{
	timer_parameter_struct timer_initpara;
   
    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);
	
	 /* TIMER0 configuration */
    timer_initpara.prescaler         = TIME0_PSC;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (Interval * ((TIM_CLK/TIME0_PSC)/1000000)-1);
//	timer_initpara.period            = 999;//1ms*10
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);
	
	nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn, 1, 1);
	
	/* Auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER0);

    /* TIMER0 channel control update interrupt enable */
    timer_interrupt_enable(TIMER0, TIMER_INT_UP);

    /* TIMER0 counter enable */
    timer_enable(TIMER0);
}

//蜂鸣器音调设置.修改频率
void BSPBeepLenght(uint32_t Rate)
{

	if(Rate > TIM_CLK / (TIME1_PSC+1))//当频率大于最大频率时，就只能等于最大频率
	{
		Rate = TIM_CLK / (TIME1_PSC+1);
	}
	else if(Rate < 1)//最小为1Hz
	{
		Rate = 1;
	}	
	
	//频率=TIM_CLK/{(ARR+1)*(PSC+1)} ==> arr=(TIM_CLK/(PSC+1)/频率)-1;
	ReloadValue = (TIM_CLK / (TIME1_PSC+1) / Rate) - 1;
}

//蜂鸣器音量设置.修改占空比
void BSPBeepVol(uint16_t DutyRatio)
{
	DutyCycle = DutyRatio;
	
	if(DutyCycle > 100)//占空比最大100%
	{
		DutyCycle = 100;
	}
	
//	DutyCycle = DutyRatio;//占空比
}

/***************************************************
作用：	蜂鸣器播放.
参数：	Time-时长
返回:	无
***************************************************/
void BSPBeepPlay(uint32_t Time)
{
	BSPBeepLenght(ScanSet.BeepLenght);
	BSPBeepVol(ScanSet.BeepVol);
	
	BeepTimeSet(Time);
}

//蜂鸣器按音调响起.
void BeepSoundTone(uint8_t *Song, uint8_t *Beat)
{
	uint8_t i=0;
	while(Song[i]<16)
	{
		if(!BeepCountTime)
		{
			BSPBeepLenght(Tone2[Song[i]]);
			BSPBeepVol(ScanSet.BeepVol);
			BeepTimeSet(Beat[i]);
			i++;
		}
	}
}

//设置前缀后缀等等时的蜂鸣声
void SetBeepTone(void)
{
	BeepSoundTone(Song_NewSetBeep, Beat_NewSetBeep);
}

//设置出错提示音
void ErrorBeepSound(Beep_Type beep)
{
	if(!beep)
	{
		BeepSoundTone(Song_NewErrorBeep,Beat_NewErrorBeep);
	}
	else
	{
		BeepSoundTone(Song_NewSetErrorBeep,Beat_NewSetErrorBeep);
	}
}

/***************************************************
作用：	蜂鸣器时间设置.
参数：	Time-时长
返回:	无
***************************************************/
static void BeepTimeSet(uint32_t Time)
{
	BeepTime = Time;
}

/***************************************************
作用：	蜂鸣器时间获得.
参数：	无
返回:	BeepTime
***************************************************/
static uint32_t BeepTimeGet(void)
{
	return(BeepTime);
}

/***************************************************
作用：	蜂鸣器播放任务.
参数：	无
返回:	无
***************************************************/
static void BeepPlayRoutine(void)
{
	static uint8_t beepState = TRUE;
	
	if(0 == BeepCountTime--)//time--; if(time == 0)
	{
		BeepCountTime = BeepTimeGet();
		beepState = ScanSet.BeepSound;//获取蜂鸣器状态
		if(BeepCountTime && beepState)//蜂鸣时间和蜂鸣器状态
		{
			timer_enable(TIMER1);
			timer_autoreload_value_config(TIMER1, ReloadValue);//配置定时器自动加载寄存器值
			timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, ReloadValue*DutyCycle/100);//配置捕获值
			
			BeepTimeSet(0);//参数读取之后,就将Time = 0;这样就只读一次了.
		}
		else
		{
			timer_disable(TIMER1);
		}
	}
}


/*******************************************************************
//作用：定时器添加中断程序
//参数：Function-添加当定时器中断时执行的程序，最多添加8个
//返回：TRUE-成功
//      FALSE-失败 
*******************************************************************/
bool BSPTimer0AddFunc(CallBackISR Function)
{
    static uint8_t count = 0;

    if(count < CALL_BACK_MAX)
    {
        ISR[count++] = Function;
        return(TRUE);
    }
	return(FALSE);
}


void TIMER0_BRK_UP_TRG_COM_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER0 ,TIMER_INT_FLAG_UP))
	{		
		uint8_t i = 0;
	
		while(i < CALL_BACK_MAX)
		{
			ISR[i++]();
		}
		
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);	//clear
	}
}


