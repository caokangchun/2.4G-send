#ifndef	__FIFO_H__
#define	__FIFO_H__


#include "Common.h"


//定义FIFO类型
typedef struct FIFO_* FIFO;


//定义FIFO写数据回调函数类型
typedef void(* FIFOWrite)(void* DataTo, const void* DataFrom);


//FIFO建立
extern FIFO FIFOCreate(uint16_t DataNumber, uint8_t DataLength);
//FIFO写数据回调函数配置
extern void FIFOCallbackWrite(FIFO Thiz, FIFOWrite Write);
//FIFO压入数据
extern bool FIFOPush(FIFO Thiz, const void* Data);
//FIFO弹出数据
extern bool FIFOPop(FIFO Thiz, void* Data);
//获得FIFO所含数据个数
extern uint16_t FIFOCount(FIFO Thiz);
//清除FIFO指定个数数据
extern bool FIFOSpecifyClear(FIFO Thiz, uint16_t Count);
//清空整个FIFO数据
extern bool FIFOClear(FIFO Thiz);
//销毁FIFO
extern bool FIFODestroy(FIFO* Thiz);


#endif
