#include "BSP.h"
static uint8_t* HidRxBuff;
static bool HidRxFlag = FALSE;
uint8_t* GetHidRxBuff(void)
{
	return HidRxBuff;
}
void ConfigHidRxBuff(uint8_t * ary)
{
	HidRxBuff = ary ;
}

void SetHidRxFlag(void)
{
	HidRxFlag = TRUE;
}

void ClrHidRxFlag(void)
{
	HidRxFlag = FALSE;
}

bool GetHidRxFlag(void)
{
	return HidRxFlag;
}

void BSPHIDInit(uint8_t interval, uint8_t power)
{ 
	uint32_t time = 0;
    Set_System();
	
	USB_Interrupts_Config();
	Set_USBClock();
	
    USB_Cable_Config(DISABLE);

    USB_Init(0);
	while(CONFIGURED!=bDeviceState)
	{
		time++;
		if(time>5000000)
		{
			break;
		}
	}
	USB->rEP1_CTRL = 0;
	USB->rEP2_CTRL = 0;
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
    USB_Istr();
}

static void MyMemcoy(uint8_t* toArray,const uint8_t* fromArray,uint8_t len)
{
	__IO uint8_t i = 0;
	for(i = 0;i < len ;i++)
	{
		toArray[i] = fromArray[i];
	}
}	

static void MyMemset(uint8_t* toArray,uint8_t num,uint8_t len)
{
    while (len > 0)
	{
		*toArray++ = (uint8_t) num;
		--len;
    }
}

bool BSPHIDReport( uint8_t* keybuff, uint8_t len )
{
	uint8_t tempBuffer[8];
	memset(tempBuffer, 0, 8);
	if(len> 8||len == 0)
	{
		return FALSE;
	}		
	else if(len == 8)
	{
		MyMemcoy(tempBuffer,keybuff,8);
		UserToPMABufferCopy(tempBuffer, ENDP1, 8);    
		/* Enable endpoint for transmission */
		_SetUSB_CTRL1(EP1_CTRL_TRANEN | 8) ;
		/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/	
	}
    else
	{
		MyMemcoy(tempBuffer,keybuff,len);
		MyMemset(tempBuffer+len,0,8-len);
		UserToPMABufferCopy(tempBuffer, ENDP1, 8);    
		/* Enable endpoint for transmission */
		_SetUSB_CTRL1(EP1_CTRL_TRANEN | 8) ;
		/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/	
	}  
	return TRUE;	
}

bool BSPHIDWaitOver(void)
{
    uint32_t timeout = 0;
    while(_GetUSB_CTRL1() & EP1_CTRL_TRANEN)
    {
        if(++timeout > 50000)
        {
            _SetUSB_CTRL1(0);
			return FALSE;
        }
    }
    _SetUSB_CTRL1(0);
	return TRUE;
}
void BSPHIDClrResidueKey(void)
{
	uint8_t i[8] = {0,0,0,0,0,0,0,0};
	BSPHIDReport(i,8);
	BSPHIDWaitOver();
}
void BSPHIDCustomReport(uint8_t* txBuff)
{
	UserToPMABufferCopy(txBuff, ENDP2, CUSTOMHID_DATA_SIZE);    
	_SetUSB_CTRL2(EP2_CTRL_TRANEN | CUSTOMHID_DATA_SIZE) ;
}
bool BSPHIDCustomWaitOver(void)
{
    uint32_t timeout = 0;
    while(_GetUSB_CTRL2() & EP2_CTRL_TRANEN)
    {
        if(++timeout > 50000)
        {
            _SetUSB_CTRL2(0);
			return FALSE;
        }
    }
    _SetUSB_CTRL2(0);
	return TRUE;
}
bool BSPHIDConnet(void)
{
	uint32_t time = 0;
	if((_GetUSB_TOP_STA()&0x02) == 0x00)
	{
		PowerOn();
		bDeviceState = UNCONNECTED;
		while(CONFIGURED!=bDeviceState)
		{
			time++;
			if(time>5000000)
			{
				break;
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
bool BSPHIDDisconnet(void)
{
	if((_GetUSB_TOP_STA()&USB_TOP_CONNECT))
	{
		PowerOff();
		return TRUE;
	}
	else
	{
		return FALSE ;
	}
	
}
bool BSPHIDGetUSBConState(void)
{
	if(bDeviceState == CONFIGURED)
	{
		if((_GetUSB_TOP_STA()&USB_TOP_CONNECT))
		{
			return TRUE;
		}
		else if((_GetUSB_TOP_STA()&USB_TOP_CONNECT))
		{
			return FALSE ;
		}
	}	
	else if(bDeviceState == SUSPENDED)
	{
		PowerOff();
		return FALSE;
	}
	return FALSE ;
}

