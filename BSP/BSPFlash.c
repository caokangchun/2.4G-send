#include <stdint.h>
#include "Common.h"
#include "W25Q16.h"
#include "BSP.h"


void BSPFlashInit(void)
{
	W25Q16Init();
}

bool BSPFlashOpen(void)
{
	return(W25Q16Open());
}

void BSPFlashClose(void)
{
	W25Q16Close();
}

void BSPFlashReset(void)
{
	W25Q16Reset();
}

void BSPFlashSectorErase(uint32_t Address)
{
	W25Q16SectorErase(Address);
}

void BSPFlashBlockErase(uint32_t Address)
{
	W25Q16BlockErase(Address);
}

void BSPFlashClipErase(void)
{
	W25Q16ClipErase();
}

void BSPFlashPageProgram(uint32_t Address, const uint8_t* Data, uint16_t Length)
{
	W25Q16PageProgram(Address, Data, Length);
}

void BSPFlashRead(uint32_t Address, uint8_t* Data, uint16_t Length)
{
	W25Q16Read(Address, Data, Length);
}




