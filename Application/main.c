#include <stdio.h>
#include <string.h>
#include "BSP.h"
#include "ascii2hid.h"
//#include "SEGGER_RTT.h"
#include "includes.h"
#include "ConfigAddr.h"



//#define DEBUG_CKC 	1

#ifdef DEBUG_CKC

typedef struct
{
	uint16_t SendCount;
	uint16_t SuccessCount;
	uint16_t FailCount;
	uint16_t FailIndex[1000];
}Record_Transmit;

Record_Transmit RecordTransmit={0,0,0,{0}};


#endif

static uint8_t ScanDelay;


//void delay_1ms(uint16_t n)
//{
//	while(n--)
//	{
//		BSPTimerDelay(500);	//us
//		BSPTimerDelay(500);	//us
//	}
//}

static void LEDFlash(void)
{
	static bool on = TRUE;
	
	if(on == TRUE)
	{
		BSPLEDOn(LEDR);
		on = FALSE;
	}
	else	
	{
		BSPLEDOff(LEDR);
		on = TRUE;
	}
}

static void ScanCode(void)
{
	BSPCodeScannerScanStart();
	ScanDelay = 1;
}

static void Timer(void)
{
	//按键按下扫描头工作，松开后延迟100ms将其关闭
	if(ScanDelay != 0)
	{
		--ScanDelay;
	}
	else
	{
		BSPCodeScannerScanStop();
	}
}


/*******************************************以下用于测试  ckc************************************************/
static uint8_t Datas[700];
static uint16_t DataLen=0;
static uint16_t Tcount=0;
static uint32_t Scount=0;

bool IsTestOver(void)
{
	char result[] = "0.40mm\r\n";
	char i=0;
	
	for(i=0;i<sizeof(result)-1;i++)
	{
		if(result[i] != Datas[i])
			return FALSE;
	}
	return TRUE;
}



bool IsRequestMatch(void)
{
	char result[] = "0.36mm\r\n";
	char i=0;
	
	for(i=0;i<sizeof(result)-1;i++)
	{
		if(result[i] != Datas[i])
			return FALSE;
	}
	return TRUE;
}


bool IsDataCorrect(void)
{
//	char result[] = "0123456789\r\n";
//	char result[] = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n";		//100
	
	//300
	char result[] = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n";
	uint16_t i=0;
	
	for(i=0;i<sizeof(result)-1;i++)
	{
		if(result[i] != Datas[i])
			return FALSE;
	}
	return TRUE;
}


static void LedFlash(void)
{
	static bool LedStatus = 0;
	
	if(!LedStatus)
	{
		BSPLEDOn(LEDR);
	}
	else
	{
		BSPLEDOff(LEDR);
	}
	LedStatus = !LedStatus;
}



#define ISR_NUM_INVAILD	0xf0

void RequestMatch()
{
	static uint8_t ISRNum = ISR_NUM_INVAILD;
	
	
	ISRNum = BSPTimerIntervalCreate(LedFlash,200);
	
	
	if(BSPWirelseeRequestMatch())
	{
		ScanCode();delay_1ms(100);
		BSPCodeScannerScanStop();delay_1ms(100);
		ScanCode();delay_1ms(100);
		BSPCodeScannerScanStop();delay_1ms(100);		
	}
	
	BSPTimerIntervalDestroy(ISRNum);
	BSPLEDOn(LEDR);
}

void test()
{
	if(BSPCodeScannerAllowRead() == TRUE)
	{
		Datas[DataLen++] = BSPCodeScannerRead();
		
		if(Datas[DataLen-1] == 0x0A)
		{
			//分析是否为结束
				//保存
				if(IsTestOver())
				{
					BSPEEPROMWrite(CONFIG_SuccessCount, (uint16_t*)&Scount, 2);
					DataLen=0;
					BSPCodeScannerScanStop();
					delay_1ms(50);
					Tcount=0;
					return;
				}
				
				if(IsRequestMatch())
				{
					RequestMatch();
					DataLen=0;
					BSPCodeScannerScanStop();
					delay_1ms(50);
					Tcount=0;
					return;
				}
				
//				if(!IsDataCorrect())
//				{
//					while(1)
//					{
//						BSPLEDOn(LEDR);	
//						delay_1ms(200);
//						BSPLEDOff(LEDR);
//						delay_1ms(200);
//					}
//				}
				
		#ifdef DEBUG_CKC
				while(!BSPWirelessWriteBytes(Datas,DataLen))
				{
					RecordTransmit.SendCount++;	//发送次数++
					RecordTransmit.FailIndex[RecordTransmit.FailCount] = RecordTransmit.SendCount; //记录index
					RecordTransmit.FailCount++;	//失败次数++
	
				}
				RecordTransmit.SendCount++;	//发送次数++
				RecordTransmit.SuccessCount++;	//成功次数++

		#else
				while(!BSPWirelessWriteBytes(Datas,DataLen));
				
				
		#endif
				Scount++;
				DataLen=0;
				BSPCodeScannerScanStop();
				BSPLEDOff(LEDR);
				delay_1ms(50);
				BSPLEDOn(LEDR);
				Tcount=0;				
		}
	}
	else
	{
		ScanCode();
		delay_1ms(1);
		Tcount++;
		if(Tcount>3000)
		{
			BSPCodeScannerScanStop();
			delay_1ms(3000);
			Tcount=0;
		}
	}	
}


char xxx[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//char xxx[] = "0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij0123456789abcdefghij";
//uint8_t xxx[] = {0xff,0xc8,0xfd,0xff,0xc2,0xeb,'S','A','N','M','A'};
//uint8_t xxx[] = {'S','A','N','M','A'};
//uint8_t xxx[][2] = {{0x20,0x14},{0x00,0x00},{0x20,0x15},{0x00,0x00},{0x00,0x2c},{0x00,0x00},{0x20,0x17},{0x00,0x00},{0x20,0x8},{0x00,0x00},{0x20,0x16},{0x00,0x00},{0x20,0x17},{0x00,0x00},{0x00,0x28},{0x00,0x00}};
void xxxxxtest(void)
{
//	if(Scount == 100000)
	if(Scount == 500)	
	{
		while(1);
	}
	while(!BSPWirelessWriteBytes((const uint8_t*)xxx,sizeof(xxx)))
	{
		__nop();
	}
	Tcount=0;
	Scount++;
//	delay_1ms(1000);
}




static UINT16 getGbk(uint8_t **buff, UINT16 *len)
{
	UINT16 gbk;
	uint8_t *dataPtr = *buff;	  //获取数据指针


	if(0xff != *dataPtr)
	{
		gbk = *dataPtr;
		*len = *len-1;	//长度-1

		*buff = *buff+1;	  //调整数据指针位置
	}
	else
	{
		dataPtr++;		//数据指针+1，指向实际数据

		gbk = *dataPtr++;
		gbk <<= 8;
		gbk += *dataPtr++;
		*len = *len-3;	//长度-3

		*buff = *buff+3;	//调整数据指针位置
	}

	return gbk;
}


static uint8_t KeyboardVal[22]={0}; 
static BOOL gbk2Keyval(UINT16 gbk, uint8_t *keyval, uint8_t *len)
{
	uint8_t lenTmp=0,i;
	uint8_t key[5];

	/*转换12345 -> key[]={1,2,3,4,5}*/
    key[4] = gbk % 10;
    gbk /= 10;
    key[3] = gbk % 10;
    gbk /= 10;
    key[2] = gbk % 10;
    gbk /= 10;
    key[1] = gbk % 10;
    gbk /= 10;
    key[0] = gbk % 10;

	/*转换为键值*/
    for(i = 0; i < 5; i++)
    {
        if(key[i] == 0)
        {
            key[i] = hid_nk_0;
        }
        else
        {
            key[i] = key[i]-1+hid_nk_1;
        }

		keyval[lenTmp++] = 0x40;	keyval[lenTmp++] = key[i];	//按下
		keyval[lenTmp++] = 0x40;	keyval[lenTmp++] = 0;		//释放
    }
	keyval[lenTmp++] = 0;	keyval[lenTmp++] = 0;				//释放all
	
	*len = lenTmp;

	return TRUE;
}

/************************************************end*****************************************************/
int main(void)
{
	uint8_t *ptr = (uint8_t*)xxx;
	uint16_t len;
	uint16_t gbk;
	uint8_t len1;
	
	Key_Signal keySig;
	uint8_t chr;

	
	BSPPowerInit();
	BSPPowerOn();
	
	BSPSystemInit();
	BSPSystemIntOpen();
	
	BSPTimerInit();
	
	BSPKeyInit();
	BSPKeyModeSet(BSP_KEY_MODE_PRESS);
	BSPKeyEnable();
	BSPTimerIntervalCreate(BSPKeyRoutine, 20);
	while(BSPKeyRead().Type != BSP_KEY_TYPE_NONE);
	
	BSPEEPROMInit(MCU_FLASH_SIZE, EEPROM_SIZE);
	
	BSPFlashInit();
	BSPFlashOpen();
	
	BSPCodeScannerInit();
	BSPCodeScannerOpen();
	
	BSPLEDInit(LEDR);
	
	BSPSoundInit(10);

	BSPHIDInit(0x01, 0xFA);

	//SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
	//SEGGER_RTT_WriteString(0, "Wifi module debug display\r\n\r\n");
	
	BSPTimerIntervalCreate(Timer, 100);

	
	BSPWirelessInit();
	BSPEEPROMReadBytes(CONFIG_SuccessCount, (uint16_t*)(&Scount), 2);	//读取成功记录
	if(Scount == 0xffffffff) 
	{
		Scount=0;	
	}
//	gbk=getGbk(&ptr,&len);
//	gbk2Keyval(gbk,KeyboardVal,&len1);
//	delay_1ms(5000);


	while(1)
	{
//		RequestMatch();
		
//		test();
		xxxxxtest();	
//		if(BSPKeyAllowRead() == TRUE)
//		{
//			keySig = BSPKeyRead();
//			if(keySig.Type == BSP_KEY_TYPE_LONG)
//			{
//				BSPPowerOff();
//			}
//			else if(keySig.Type == BSP_KEY_TYPE_SHORT)
//			{
//				LEDFlash();
//	
//				ScanCode();
//			}
//			
//		}
//		
//		if(BSPCodeScannerAllowRead() == TRUE)
//		{
//			chr = BSPCodeScannerRead();

//			USB_KeySend(&chr, 1);
//			
//			if(chr == 0x0D)
//			{
//				BSPSoundPlay(MUSIC_TRACK_1);
//			}
//		}
		
		

		BSPRoutine();
	}
}



