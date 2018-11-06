
#ifndef _DRV_EXTI_H_
#define _DRV_EXTI_H_

#include "HAL_exti.h"

#define CLK_LINE        EXTI_Line13
#define DATA_LINE       EXTI_Line14

void EXTI_Configuration(void);
void EXTI_IntEnable(uint32_t EXTI_Line);
void EXTI_IntDisable(uint32_t EXTI_Line);
void EXTI_EdgeConfig(uint32_t EXTI_Line, EXTITrigger_TypeDef Trigger);

#endif /* end of _DRV_EXTI_H_ */


