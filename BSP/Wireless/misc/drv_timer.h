
#ifndef _DRV_TIMER_H_
#define _DRV_TIMER_H_

#include "HAL_tim.h"

/* Exported macro ----------------------------------------------- */
#define SYS_TIM_START() TIM_Cmd(TIM3, ENABLE);
#define SYS_TIM_STOP()  TIM_Cmd(TIM3, DISABLE);

#define CAP_Enable()    TIM_Cmd(TIM3, ENABLE);
#define CAP_Disable()   TIM_Cmd(TIM3, DISABLE);
#define CAP_CNT_CLR()   TIM_SetCounter(TIM3, 0);
#define CAP_CNT_GET()   TIM_GetCounter(TIM3)

#define BUZZ_FREQ(a)    PWM_Frequency(a)

/* Exported functions ------------------------------------------- */
void TIMER_Configuration(uint16_t us_timer);
void PWM_Configuration(uint32_t freq, uint8_t duty);
void PWM_Enable(void);
void PWM_Disable(void);
void PWM_Frequency(uint32_t freq);
void CAP_Configuration(uint16_t us_timer);

#endif /* end of _DRV_TIMER_H_ */

