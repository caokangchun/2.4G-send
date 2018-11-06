#ifndef	__W25Q16_H__
#define	__W25Q16_H__


#include <stdint.h>
#include "Common.h"
#include "BSP.h"


extern void W25Q16Init(void);
extern bool W25Q16Open(void);
extern void W25Q16Close(void);
extern void W25Q16Reset(void);
extern void W25Q16SectorErase(uint32_t Address);
extern void W25Q16BlockErase(uint32_t Address);
extern void W25Q16ClipErase(void);
extern void W25Q16PageProgram(uint32_t Address, const uint8_t* Data, uint16_t Length);
extern void W25Q16Read(uint32_t Address, uint8_t* Data, uint16_t Length);


#endif



