#ifndef	__FLASH_H__
#define	__FLASH_H__


#include <stdint.h>
#include "Common.h"
#include "BSP.h"


extern void BSPFlashInit(void);
extern bool BSPFlashOpen(void);
extern void BSPFlashClose(void);
extern void BSPFlashReset(void);
extern void BSPFlashSectorErase(uint32_t Address);
extern void BSPFlashBlockErase(uint32_t Address);
extern void BSPFlashClipErase(void);
extern void BSPFlashPageProgram(uint32_t Address, const uint8_t* Data, uint16_t Length);
extern void BSPFlashRead(uint32_t Address, uint8_t* Data, uint16_t Length);




#endif



