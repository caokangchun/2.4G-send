#ifndef	__DLLIST_H__
#define	__DLLIST_H__

#include "MM32F103.h"
#include "Common.h"


//定义DLList类型
typedef struct DLList_* DLList;


//定义DLList写数据回调函数类型 
typedef void(* DLListWrite)(void* DataTo, const void* DataFrom);
//定义DLList比较数据回调函数类型 
typedef bool(* DLListEqual)(const void* DataA, const void* DataB);


//DLList建立
extern DLList DLListCreate(uint16_t NodeNumber, uint8_t NodeLength);
//DLList写数据回调函数配置
extern void DLListCallbackWrite(DLList Thiz, DLListWrite Write);
//DLList比较数据回调函数配置
extern void DLListCallbackEqual(DLList Thiz, DLListEqual Write);
//向DLList增加一个节点 
extern bool DLListNodeAdd(DLList Thiz, uint16_t Position, const void* Data);
//向DLList头增加一个节点 
extern bool DLListNodeHeadAdd(DLList Thiz, const void* Data);
//向DLList尾增加一个节点 
extern bool DLListNodeTailAdd(DLList Thiz, const void* Data);
//向DLList删除一个节点 
extern bool DLListNodeDel(DLList Thiz, uint16_t Position);
//向DLList头删除一个节点 
extern bool DLListNodeHeadDel(DLList Thiz);
//向DLList尾删除一个节点 
extern bool DLListNodeTailDel(DLList Thiz);
//向DLList写（修改）一个节点 
extern bool DLListNodeWrite(DLList Thiz, uint16_t Position, const void* Data);
//向DLList头写（修改）一个节点 
extern bool DLListNodeHeadWrite(DLList Thiz, const void* Data);
//向DLList尾写（修改）一个节点 
extern bool DLListNodeTailWrite(DLList Thiz, const void* Data);
//向DLList读一个节点 
extern bool DLListNodeRead(DLList Thiz, uint16_t Position, void* Data);
//向DLList头读一个节点 
extern bool DLListNodeHeadRead(DLList Thiz, void* Data);
//向DLList尾读一个节点 
extern bool DLListNodeTailRead(DLList Thiz, void* Data);
//移动DLList节点
extern bool DLListNodeMove(DLList Thiz, uint16_t PositionTo, uint16_t PositionFrom);
//移动节点至DLList头 
extern bool DLListNodeHeadMove(DLList Thiz, uint16_t PositionFrom);
//移动节点至DLList尾 
extern bool DLListNodeTailMove(DLList Thiz, uint16_t PositionFrom);
//交换DLList两个节点 
extern bool DLListNodeSwap(DLList Thiz, uint16_t PositionA, uint16_t PositionB);
//节点交换至DLList头 
extern bool DLListNodeHeadSwap(DLList Thiz, uint16_t Position);
//节点交换至DLList尾 
extern bool DLListNodeTailSwap(DLList Thiz, uint16_t Position);
//逆序DLList所有节点 
extern bool DLListNodeInvert(DLList Thiz);
//获得DLList所含节点个数 
extern uint16_t DLListNodeCount(DLList Thiz);
//搜索DLList上含有相同数据的第一个节点号 
extern int32_t DLListNodeSearch(DLList Thiz, const void* Data);
//清空整个DLList 
extern bool DLListClear(DLList Thiz);
//销毁DLList 
extern bool DLListDestroy(DLList* Thiz);


#endif
