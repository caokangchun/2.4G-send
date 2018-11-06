#ifndef	__BEEP_H__
#define	__BEEP_H__


#include "Common.h"
#include "BSP.h"
#include "ActionConfig.h"

//Hz
#if 0
#define DO_L	262
#define RE_L 	294
#define MI_L 	330
#define FA_L 	349
#define SO_L 	392
#define LA_L 	440
#define SI_L 	494

#define DO_M	523
#define RE_M 	587
#define MI_M 	659
#define FA_M 	698
#define SO_M 	784
#define LA_M 	880
#define SI_M 	988

#define DO_H 	1047
#define RE_H 	1175
#define MI_H 	1319
#define FA_H 	1397
#define SO_H 	1568
#define LA_H 	1760
#define SI_H 	1967

#endif

/***************************************************
//作用：	蜂鸣器初始化.使用了定时器0,定时器1,和蜂鸣器.
//参数：	无
//返回:	无
***************************************************/
extern void BSPBeepInit(void);
/***************************************************
作用：	蜂鸣器播放.
参数：	Time-时长
返回:	无
***************************************************/
extern void BSPBeepPlay(uint32_t Time);
/*******************************************************************
//作用：定时器添加中断程序
//参数：Function-添加当定时器中断时执行的程序，最多添加8个
//返回：TRUE-成功
//      FALSE-失败 
*******************************************************************/
extern bool BSPTimer0AddFunc(CallBackISR Function);//蜂鸣定时器0添加中断程序
/*******************************************************************
//作用：设置前缀后缀等添加字符时的蜂鸣声
//参数：无
//返回：无
*******************************************************************/
extern void SetBeepTone(void);
/*******************************************************************
//作用：设置出错提示音
//参数：音效选择
//返回：无
*******************************************************************/
extern void ErrorBeepSound(Beep_Type beep);



#endif


