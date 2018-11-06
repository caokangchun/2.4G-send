#include <stdint.h>
#include "BSP.h"


void BSPRoutine(void)
{		
	//BSPWatchDogReset();
	//BSPKeyRoutine();
	BSPCodeScannerRoutine();
	BSPWirelessRoutine();
}

