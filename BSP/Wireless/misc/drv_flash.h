
#ifndef _DRV_FLASH_H_
#define _DRV_FLASH_H_

#include "HAL_flash.h"

/* Exported functions -------------------------------------------------------------- */
void Flash_ProgramBytes(uint32_t StartAddr, uint8_t *Bytes, uint16_t Length);
void Flash_ErasePage(uint32_t SectorAddr);
void Flash_WriteBytes(uint32_t StartAddr, uint8_t *Bytes, uint16_t Length);
void Flash_ReadBytes(uint32_t StartAddr, uint8_t *Bytes, uint16_t Length);

#endif /* end of _DRV_FLASH_H_ */
