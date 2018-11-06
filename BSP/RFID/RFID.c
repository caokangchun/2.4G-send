#include <stdlib.h>
#include "Common.h"
#include "MM32F103.h"
#include "BSP.h"
#include "UART2.h"
#include "FIFO.h"
#include "RFID.h"


static FIFO UIDRead;
static uint16_t TimerValue = 0;


static void SetTimer(uint16_t Time)
{
	BSPSystemIntClose();
	TimerValue = Time;
	BSPSystemIntOpen();
}

static bool IsTimerArrive()
{
	BSPSystemIntClose();
	if(TimerValue == 0)
	{
		BSPSystemIntOpen();
		return(TRUE);
	}
	BSPSystemIntOpen();
	return(FALSE);
}

static void Timer(void)
{
	if(TimerValue > 0)
	{
		--TimerValue;
	}
}

static void WriteData(void* DataTo, const void* DataFrom)
{
	*((UID_Data*)DataTo) = *((UID_Data*)DataFrom);
}

void RFIDInit(void)
{
	UART2Init(9600);
	UART2Open();
	
	UIDRead = FIFOCreate(8, sizeof(UID_Data));
	FIFOCallbackWrite(UIDRead, WriteData);
	
	BSPTimerIntervalCreate(Timer, 10);
}

bool RFIDAllowRead(void)
{
	if(FIFOCount(UIDRead) > 0)
	{
		return(TRUE);
	}
	return(FALSE);
}

UID_Data RFIDRead(void)
{
	UID_Data tmp;
	
	FIFOPop(UIDRead, &tmp);
	return(tmp);
}

void RFIDRoutine(void)
{
	static uint8_t state = 0;
	static uint8_t index;
	static UID_Data uid;
	uint8_t data;
	
	UART2Routine();
	
	if(IsTimerArrive() == TRUE)
    {
        state = 0;
    }
	SetTimer(100);
	
	if(UART2AllowRead() == FALSE)
	{
		return;
	}
	data = UART2Read();

	switch(state)
	{
		case 0:
		{
			if(data == 0x02)
			{
				index = 0;
				state = 1;
			}
			break;
		}
		case 1:
		{
			uid.Data[index] = data;
			if(++index == 8)
			{
				state = 2;
			}
			break;
		}
		case 2:
		{
			state = 3;
			break;
		}
		case 3:
		{
			if(data == 0x03)
			{
				FIFOPush(UIDRead, &uid);
			}
			state = 0;
			break;
		}
	}
}
