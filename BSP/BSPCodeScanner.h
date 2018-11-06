#ifndef	__CODESCANNER_H__
#define	__CODESCANNER_H__


#include "Common.h"
#include "BSP.h"


extern void BSPCodeScannerInit(void);
extern void BSPCodeScannerOpen(void);
extern void BSPCodeScannerClose(void);
extern void BSPCodeScannerScanStart(void);
extern void BSPCodeScannerScanStop(void);
extern bool BSPCodeScannerAllowRead(void);
extern uint8_t BSPCodeScannerRead(void);
extern void BSPCodeScannerRoutine(void);


#endif



