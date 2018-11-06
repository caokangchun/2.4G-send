#ifndef	__LED_H__
#define	__LED_H__


#include "Common.h"
#include "BSP.h"


/* exported types */
typedef enum 
{
	LEDR,
}
Led_Enum;


/***************************************************
作用：	LED灯初始化.
参数：	LedNum：LED1或LED2
返回:	无
***************************************************/
extern void BSPLEDInit(Led_Enum LedNum);

/***************************************************
作用：	LED灯打开.
参数：	LedNum： LED1或LED2
返回:	无
***************************************************/
extern void BSPLEDOn(uint8_t LedNum);

/***************************************************
作用：	LED灯关闭.
参数：	LedNum： LED1或LED2
返回:	无
***************************************************/
extern void BSPLEDOff(uint8_t LedNum);

/***************************************************
作用：	LED灯闪烁打开.
参数：	LedNum： LED1或LED2
		time:闪烁间隔,单位:100ms
返回:	无
备注:	使用LED灯闪烁功能需要将BSPLEDFlashRoutine()添加至定时器.
***************************************************/
extern void BSPLEDFlashOn(Led_Enum LedNum, uint16_t time);

/***************************************************
作用：	LED灯闪烁关闭.
参数：	LedNum： LED1或LED2
返回:	无
备注:	使用LED灯闪烁功能需要将BSPLEDFlashRoutine()添加至定时器.
***************************************************/
extern void BSPLEDFlashOff(Led_Enum LedNum);

/***************************************************
作用：	LED灯周期任务.
参数：	无
返回:	无
***************************************************/
extern void BSPLEDFlashRoutine(void);


#endif



