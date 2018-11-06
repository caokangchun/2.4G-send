#ifndef	__WATCHDOW_H__
#define	__WATCHDOW_H__


#include "Common.h"
#include "BSP.h"


//�������Ź�����
typedef enum 
{
	MS409,
	MS819,
	MS1638,
	MS3276,
	MS6553,
	MS13107,
	MS26214,
}WDTInterval;


/*******************************************************************
���ã����Ź���ʼ����λʱ�䣬����ʼ����
������
	MS409,
	MS819,
	MS1638,
	MS3276,
	MS6553,
	MS13107,
	MS26214,
���أ���
*******************************************************************/
extern void BSPWatchDogInit(WDTInterval time);




/*******************************************************************
���ã�ι������
��������
���أ���
*******************************************************************/
extern void BSPWatchDogReset(void);


#endif


