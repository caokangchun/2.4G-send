#ifndef	__UART_H__
#define	__UART_H__


#include "Common.h"
#include "BSP.h"


//*******************************************************************
//作用：UART初始化
//参数：BPS-波特率
//返回：无
//*******************************************************************
extern void UART0Init(uint32_t BPS);
//*******************************************************************
//作用：UART添加读中断程序
//参数：Function-添加当定时器中断时执行的程序，最多添加8个
//返回：TRUE-成功
//      FALSE-失败 
//*******************************************************************
extern bool UART0AddReadISR(CallBackISR Function);
//*******************************************************************
//作用：UART添加写中断程序
//参数：Function-添加当定时器中断时执行的程序，最多添加8个
//返回：TRUE-成功
//      FALSE-失败 
//*******************************************************************
extern bool UART0AddWriteISR(CallBackISR Function);
//*******************************************************************
//作用：UART打开
//参数：无
//返回：无
//*******************************************************************
extern void UART0Open(void);
//*******************************************************************
//作用：UART关闭
//参数：无
//返回：无
//*******************************************************************
extern void UART0Close(void);
//*******************************************************************
//作用：UART写数据
//参数：Data-发送的数据
//返回：无
//*******************************************************************
extern void UART0Write(uint8_t Data);
//*******************************************************************
//作用：查询UART是否有数据可以读取
//参数：无
//返回：TRUE-有数据可读
//      FALSE-无数据可读
//*******************************************************************
extern bool UART0AllowRead(void);
//*******************************************************************
//作用：UART读数据
//参数：无
//返回：接收的数据
//*******************************************************************
extern uint8_t UART0Read(void);
//*******************************************************************
//作用：UART清空读缓冲区
//参数：无
//返回：无
//*******************************************************************
extern void UART0BufClear(void);
//*******************************************************************
//作用：UART周期性任务
//参数：无
//返回：无
//*******************************************************************
extern void UART0Routine(void);

extern void UART0SetBaud(uint32_t BPS);

extern uint32_t UART0GetReadCount(void);

#endif



