#include "MM32F103.h"
#include "Common.h"
#include "BSP.h"


#define		SOUND_USE_PWM				0


#if SOUND_USE_PWM
static const uint8_t* SoundTrack; 
static bool BeepOn = FALSE;
static uint16_t SoundLength;
static uint16_t SoundCount = 0;
static uint16_t TimerPeriod = 0;
static uint16_t PrescalerValue;
static uint16_t CCR1Value;


void BSPSoundInit(void)
{
    GPIO_InitTypeDef GPIOInit;
    NVIC_InitTypeDef NVICInit; 
    TIM_TimeBaseInitTypeDef TIMTimeBaseInit;
    TIM_OCInitTypeDef TIMOCInit;
    uint16_t channel1Pulse = 0;

    //IO口配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIOInit.GPIO_Pin = GPIO_Pin_8;
    GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOInit.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIOInit);

    //PWM配置
    TimerPeriod = (SystemCoreClock / (32000)) - 1;
    channel1Pulse = (uint16_t)(50 * (TimerPeriod - 1) / 100);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIMTimeBaseInit.TIM_Prescaler = 0;
    TIMTimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
    TIMTimeBaseInit.TIM_Period = TimerPeriod;
    TIMTimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIMTimeBaseInit.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIMTimeBaseInit);
    TIMOCInit.TIM_OCMode = TIM_OCMode_PWM2;
    TIMOCInit.TIM_OutputState = TIM_OutputState_Enable;
    TIMOCInit.TIM_OutputNState = TIM_OutputNState_Disable;
    TIMOCInit.TIM_Pulse = channel1Pulse;
    TIMOCInit.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIMOCInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIMOCInit.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIMOCInit.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &TIMOCInit);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    //初始化定时中断优先级
	NVICInit.NVIC_IRQChannel = TIM2_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInit.NVIC_IRQChannelSubPriority = 0;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICInit);

    //定时配置
	PrescalerValue = (uint16_t) (SystemCoreClock / 48000000) - 1;
	CCR1Value = 48000000 / 32000;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIMTimeBaseInit.TIM_Period = 65535;
    TIMTimeBaseInit.TIM_Prescaler = 0;
    TIMTimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIMTimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIMTimeBaseInit);
    TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
    TIMOCInit.TIM_OCMode = TIM_OCMode_Timing;
    TIMOCInit.TIM_OutputState = TIM_OutputState_Enable;
    TIMOCInit.TIM_Pulse = CCR1Value;
    TIMOCInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIMOCInit);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}

static uint8_t BeepRead(void)
{
    uint32_t tmp;
	
	if(BeepOn == TRUE)
	{
		if(SoundCount < SoundLength)
		{
            tmp = *SoundTrack;         
			++SoundTrack;
			++SoundCount;
			return(tmp);
		}
		else
		{
			SoundCount = 0;
			BeepOn = FALSE;
			TIM_Cmd(TIM2, DISABLE);
            return(128);
		}
	}
    else
    {
        TIM_Cmd(TIM2, DISABLE);
        return(128);
    }
}

void BSPSoundPlay(uint8_t Track)
{
    BeepOn = TRUE;
    if(Track == 1)
    {
        SoundTrack = WAV1;
        SoundLength = GetArrayLen(WAV1);
    }
    else if(Track == 2)
    {
        //SoundTrack = WAV2;
        //SoundLength = GetArrayLen(WAV2);
    }
    else
    {
        //SoundTrack = WAV3;
        //SoundLength = GetArrayLen(WAV3);
    }
    SoundCount = 0;

    TIM_SetCounter(TIM2, 0);
	TIM_SetCompare1(TIM2, CCR1Value);
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
    uint32_t duty = 0;
    uint16_t capture = 0;
    
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

        duty = BeepRead();
        duty *= 10000;
        duty /= 255;
        duty = (uint16_t)(duty * (uint32_t)(TimerPeriod - 1) / 10000);
        TIM1->CCR1 = (uint16_t)(duty);

        capture = TIM_GetCapture1(TIM2);
        TIM_SetCompare1(TIM2, capture + CCR1Value);
    }
}

#else


static bool BeepOn = FALSE;
static uint32_t SoundTrack;
static uint8_t SoundVolume;
static uint8_t SoundCenter;
static uint32_t SoundLength;
static uint32_t SoundCount = 0;
static uint16_t CCR1Value;


void BSPSoundInit(uint8_t Volume)
{
    GPIO_InitTypeDef GPIOInit;
	DAC_InitTypeDef  DACInit;
    NVIC_InitTypeDef NVICInit; 
    TIM_TimeBaseInitTypeDef TIMTimeBaseInit;
    TIM_OCInitTypeDef TIMOCInit;
	uint32_t tmp;
	
	uint16_t PrescalerValue;

    //IO口配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIOInit.GPIO_Pin =  GPIO_Pin_4;
    GPIOInit.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOInit);
	
	//DAC配置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_DeInit();
	DACInit.DAC_Trigger = DAC_Trigger_Software ;
    DACInit.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DACInit.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_1, &DACInit);
    DAC_Cmd(DAC_Channel_1, ENABLE);

    //初始化定时中断优先级
	NVICInit.NVIC_IRQChannel = TIM2_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = 1;
	NVICInit.NVIC_IRQChannelSubPriority = 0;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICInit);

    //定时配置
	PrescalerValue = (uint16_t) (SystemCoreClock / 48000000) - 1;
	CCR1Value = 48000000 / 32000;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIMTimeBaseInit.TIM_Period = 65535;
    TIMTimeBaseInit.TIM_Prescaler = 0;
    TIMTimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIMTimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIMTimeBaseInit);
    TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
    TIMOCInit.TIM_OCMode = TIM_OCMode_Timing;
    TIMOCInit.TIM_OutputState = TIM_OutputState_Enable;
    TIMOCInit.TIM_Pulse = CCR1Value;
    TIMOCInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIMOCInit);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
	
	tmp = 127;
	tmp *= (uint32_t)Volume;
	tmp *= 100;
	tmp /= 10000;
	SoundCenter = tmp;
	SoundVolume = Volume;

	DAC_SetChannel1Data(DAC_Align_8b_R, SoundCenter);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}

static uint8_t SoundRead(void)
{
    uint8_t data;
	uint32_t tmp;
	
	if(BeepOn == TRUE)
	{
		if(SoundCount < SoundLength)
		{
			BSPFlashRead(SoundTrack, &data, 1);
//			if(data <= 127)
//			{
//				tmp = data;
//				tmp *= (uint32_t)SoundVolume;
//				tmp *= 100;
//				tmp /= 10000;
//				data += tmp;
//				if(data > 127)
//				{
//					data = 127;
//				}
//			}
//			else
//			{
//				tmp = data;
//				tmp -= 127;
//				tmp *= (uint32_t)SoundVolume;
//				tmp *= 100;
//				tmp /= 10000;
//				data -= tmp;
//				if(data < 128)
//				{
//					data = 128;
//				}
//			}

			tmp = data;
			tmp *= SoundVolume;
			tmp *= 100;
			tmp /= 10000;
			data = tmp;

			++SoundTrack;
			++SoundCount;
			return(data);
		}
		else
		{
			SoundCount = 0;
			BeepOn = FALSE;
			TIM_Cmd(TIM2, DISABLE);
            return(SoundCenter);
		}
	}
    else
    {
        TIM_Cmd(TIM2, DISABLE);
        return(SoundCenter);
    }
}

void BSPSoundPlay(uint32_t Track)
{
	uint8_t data[4];
	
    BeepOn = TRUE;

	BSPFlashRead(Track + 0x28, data, 4);
	SoundLength = data[0];
	SoundLength |= (uint32_t)(data[1]) << 8;
	SoundLength |= (uint32_t)(data[2]) << 16;
	SoundLength |= (uint32_t)(data[3]) << 24;
	SoundTrack = Track + 0x28 + 0x04;
    SoundCount = 0;

    TIM_SetCounter(TIM2, 0);
	TIM_SetCompare1(TIM2, CCR1Value);
    TIM_Cmd(TIM2, ENABLE);
}

void BSPSoundStop(void)
{
	__disable_irq();
	SoundCount = 0;
	BeepOn = FALSE;
	TIM_Cmd(TIM2, DISABLE);
	DAC_SetChannel1Data(DAC_Align_8b_R, SoundCenter);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	__enable_irq();
}

void TIM2_IRQHandler(void)
{
    uint16_t capture = 0;
    
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		DAC_SetChannel1Data(DAC_Align_8b_R, SoundRead());
		DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);

        capture = TIM_GetCapture1(TIM2);
        TIM_SetCompare1(TIM2, capture + CCR1Value);
    }
}

#endif


