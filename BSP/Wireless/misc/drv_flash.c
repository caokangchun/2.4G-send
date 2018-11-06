
#include "drv_flash.h"

#define FLASH_PAGE_SIZE    ((uint16_t)0x400) // 1K

/**
  * @brief  FLASH Program Bytes Data.
  * @param  None
  * @retval None
  */
void Flash_ProgramBytes(uint32_t StartAddr, uint8_t *Bytes, uint16_t Length)
{
    uint32_t EraseCounter, EndAddr;
    uint32_t NbrOfPage;
    uint16_t Data;
    FLASH_Status FLASHStatus = FLASH_COMPLETE;

    /* Unlock the Flash Bank1 Program Erase controller */
    FLASH_Unlock();

    /* Define the number of page to be erased */
    NbrOfPage = (Length / FLASH_PAGE_SIZE) + (Length % FLASH_PAGE_SIZE) ? 1 : 0;

    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);     

    /* Erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * EraseCounter));
    }

    /* Program Flash Bank1 */

    if((Length % 2) == 0)
    {
        EndAddr = StartAddr + Length;

        while((StartAddr < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
        {
            Data = *(Bytes + 1);
            Data <<= 8;
            Data += *Bytes;
            FLASHStatus = FLASH_ProgramHalfWord(StartAddr, Data);
            StartAddr = StartAddr + 2;
            Bytes += 2;
        }
    }
    else
    {
        EndAddr = StartAddr + Length - 1;

        while((StartAddr < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
        {
            Data = *(Bytes + 1);
            Data <<= 8;
            Data += *Bytes;
            FLASHStatus = FLASH_ProgramHalfWord(StartAddr, Data);
            StartAddr = StartAddr + 2;
            Bytes += 2;
        }

        Data = 0xFF;
        Data <<= 8;
        Data += *Bytes;
        FLASH_ProgramHalfWord(StartAddr, Data);
    }

    FLASH_Lock();
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void Flash_ErasePage(uint32_t SectorAddr)
{
    FLASH_Unlock();

    FLASH_ErasePage(SectorAddr);
    
    FLASH_Lock();
}

/**
  * @brief  FLASH Program Bytes Data.
  * @param  None
  * @retval None
  */
void Flash_WriteBytes(uint32_t StartAddr, uint8_t *Bytes, uint16_t Length)
{
    uint32_t EndAddr;
    uint16_t Data;
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    
    FLASH_Unlock();
    
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);    

    if((Length % 2) == 0)
    {
        EndAddr = StartAddr + Length;

        while((StartAddr < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
        {
            Data = *(Bytes + 1);
            Data <<= 8;
            Data += *Bytes;
            FLASHStatus = FLASH_ProgramHalfWord(StartAddr, Data);
            StartAddr = StartAddr + 2;
            Bytes += 2;
        }
    }
    else
    {
        EndAddr = StartAddr + Length - 1;

        while((StartAddr < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
        {
            Data = *(Bytes + 1);
            Data <<= 8;
            Data += *Bytes;
            FLASHStatus = FLASH_ProgramHalfWord(StartAddr, Data);
            StartAddr = StartAddr + 2;
            Bytes += 2;
        }

        Data = 0xFF;
        Data <<= 8;
        Data += *Bytes;
        FLASH_ProgramHalfWord(StartAddr, Data);
    }
    
    FLASH_Lock();
}

/**
  * @brief  FLASH Program Bytes Data.
  * @param  None
  * @retval None
  */
void Flash_ReadBytes(uint32_t StartAddr, uint8_t *Bytes, uint16_t Length)
{
    uint16_t i;

    for(i = 0; i < Length; i++)
    {
        *Bytes = (*(__IO uint8_t*) StartAddr);
        StartAddr ++;
        Bytes ++;
    }
}

