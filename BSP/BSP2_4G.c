#include "BSP2_4G.h"




//无线通讯初始化
extern void BSP2_4GInit(void)
{
	/*写入固定值，用于初期2.4G测试*/
	xn297l.pipe_index=0x17;
	xn297l.mac[0] = 0x87;xn297l.mac[1] = 0x39;xn297l.mac[2] = 0x6d;xn297l.mac[3] = 0xac;
	/****************************/
	
	rf_init();
}


