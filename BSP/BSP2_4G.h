#ifndef __BSP2_4G_H__
#define __BSP2_4G_H__	1


#include "BSP.h"
#include "includes.h"


//无线通讯初始化
extern void BSP2_4GInit(void);

//无线通讯发送数据
extern bool BSP2_4GSendData(uint8_t *PData, uint8_t len);

extern bool BSP2_4GEnterRx(void);



//无线通讯打开
extern bool BSP2_4GOpen(void);
//无线通讯关闭
extern bool BSP2_4GClose(void);
//无线通讯复位
extern bool BSP2_4GReset(void);
//无线通讯建立连接
extern bool BSP2_4GConnet(void);
//无线通讯断开连接
extern bool BSP2_4GDisconnet(void);
//无线通讯在线状态查询
extern bool BSP2_4GOnline(void);

//无线通讯查询接收缓冲区数据量
extern uint16_t BSP2_4GBufCount(void);
//无线通讯清空接收缓冲区数据
extern void BSP2_4GBufClear(uint8_t* Data, uint16_t Count);
//无线通讯接收数据一字节
extern uint8_t BSP2_4GReadByte(void);
//无线通讯接收数据多字节
extern void BSP2_4GReadBytes(uint8_t* Data, uint16_t Count);
//无线通讯添加发送数据完成中断服务
extern bool BSP2_4GAddWriteISR(CallBackISR Program);
//无线通讯添加接收数据完成中断服务
extern bool BSP2_4GAddReadISR(CallBackISR Program);
//无线通讯循环程序
extern void BSP2_4GRoutine(void);

#endif

