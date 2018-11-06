#ifndef	__BSPWIRELESS_H__
#define	__BSPWIRELESS_H__


#include "Common.h"
#include "BSP.h"


typedef struct
{
	uint8_t prefix1;
	uint8_t prefix2;
	uint8_t sid;
	uint8_t crc;
}CMD_UPLOAD;




typedef enum 
{
	TYPE_BlueT,
	TYPE_2_4G,
}Wireless_Type;


/*************************
描述：返回当前连接类型
参数：void
返回：
	TYPE_BlueT 蓝牙
	TYPE_2_4G  2.4G
*************************/
Wireless_Type BSPGetWireleeType(void);


/**********2.4G，蓝牙初始化，以及设置回调函数**********/
extern void BSPWirelessInit(void);


/***********************************
无线通讯发送数据
参数：
	uint8_t* 指针
	uint16_t 长度
返回：
	TRUE	成功
	FALSE	失败
***********************************/
extern bool BSPWirelessWriteBytes(const uint8_t* Data, uint16_t Count);




/***********************************
描述：	2.4G 收发配对
参数：	void
返回：
	TRUE	成功
	FALSE	失败
***********************************/
extern bool BSPWirelseeRequestMatch(void);




















//无线通讯设配置
extern void BSPWirelessConfig(void);
//无线通讯打开
extern bool BSPWirelessOpen(void);
//无线通讯关闭
extern bool BSPWirelessClose(void);
//无线通讯复位
extern bool BSPWirelessReset(void);
//无线通讯建立连接
extern bool BSPWirelessConnet(void);
//无线通讯断开连接
extern bool BSPWirelessDisconnet(void);
//无线通讯在线状态查询
extern bool BSPWirelessOnline(void);

//无线通讯查询接收缓冲区数据量
extern uint16_t BSPWirelessBufCount(void);
//无线通讯清空接收缓冲区数据
extern void BSPWirelessBufClear(uint8_t* Data, uint16_t Count);
//无线通讯接收数据一字节
extern uint8_t BSPWirelessReadByte(void);
//无线通讯接收数据多字节
extern void BSPWirelessReadBytes(uint8_t* Data, uint16_t Count);
//无线通讯添加发送数据完成中断服务
extern bool BSPWirelessAddWriteISR(CallBackISR Program);
//无线通讯添加接收数据完成中断服务
extern bool BSPWirelessAddReadISR(CallBackISR Program);

//无线通讯循环程序,返回无线连接类型
extern void BSPWirelessRoutine(void);


#endif



