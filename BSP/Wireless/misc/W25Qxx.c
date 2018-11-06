
#include "W25Qxx.h"

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
uint32_t W25Qxx_Init(void)
{
    /*!< SPI configuration */
    //sFLASH_Configuration();
    sFLASH_Configuration(); // Mode 0(CPOL=0, CPHA=0)
    
    /*!< Read chip ID to check if get right flash */
    return (W25Qxx_ReadID());// == sFLASH_W25Q16_ID);
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t W25Qxx_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "RDID " instruction */
    sFLASH_SendByte(W25Q_JedecID);

    /*!< Read a byte from the FLASH */
    Temp0 = sFLASH_SendByte(W25Q_DUMMY_BYTE);

    /*!< Read a byte from the FLASH */
    Temp1 = sFLASH_SendByte(W25Q_DUMMY_BYTE);

    /*!< Read a byte from the FLASH */
    Temp2 = sFLASH_SendByte(W25Q_DUMMY_BYTE);

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void W25Qxx_WriteEnable(void)
{
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Write Enable" instruction */
    sFLASH_SendByte(W25Q_WriteEnable);

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void W25Qxx_WaitForWriteEnd(void)
{
    uint8_t flashstatus = 0;

    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Read Status Register" instruction */
    sFLASH_SendByte(W25Q_ReadStatusReg1);

    /*!< Loop as long as the memory is busy with a write cycle */
    do
    {
        /*!< Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        flashstatus = sFLASH_SendByte(W25Q_DUMMY_BYTE);
    }
    while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
} 

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void W25Qxx_EraseSector(uint32_t SectorAddr)
{
    /*!< Send write enable instruction */
    W25Qxx_WriteEnable();

    /*!< Sector Erase */
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send Sector Erase instruction */
    sFLASH_SendByte(W25Q_SectorErase);
    /*!< Send SectorAddr high nibble address byte */
    sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /*!< Send SectorAddr medium nibble address byte */
    sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /*!< Send SectorAddr low nibble address byte */
    sFLASH_SendByte(SectorAddr & 0xFF);
    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    W25Qxx_WaitForWriteEnd();
}

/**
  * @brief  Erases the specified FLASH 32K block.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void W25Qxx_EraseBlock32K(uint32_t Block32Addr)
{
    /*!< Send write enable instruction */
    W25Qxx_WriteEnable();

    /*!< Sector Erase */
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send Block32K Erase instruction */
    sFLASH_SendByte(W25Q_BlockErase32K);
    /*!< Send Block32Addr high nibble address byte */
    sFLASH_SendByte((Block32Addr & 0xFF0000) >> 16);
    /*!< Send Block32Addr medium nibble address byte */
    sFLASH_SendByte((Block32Addr & 0xFF00) >> 8);
    /*!< Send Block32Addr low nibble address byte */
    sFLASH_SendByte(Block32Addr & 0xFF);
    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    W25Qxx_WaitForWriteEnd();
}

/**
  * @brief  Erases the specified FLASH 64K block.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void W25Qxx_EraseBlock64K(uint32_t Block64Addr)
{
    /*!< Send write enable instruction */
    W25Qxx_WriteEnable();

    /*!< Sector Erase */
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send Block64K Erase instruction */
    sFLASH_SendByte(W25Q_BlockErase64K);
    /*!< Send Block64Addr high nibble address byte */
    sFLASH_SendByte((Block64Addr & 0xFF0000) >> 16);
    /*!< Send Block64Addr medium nibble address byte */
    sFLASH_SendByte((Block64Addr & 0xFF00) >> 8);
    /*!< Send Block64Addr low nibble address byte */
    sFLASH_SendByte(Block64Addr & 0xFF);
    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    W25Qxx_WaitForWriteEnd();
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void W25Qxx_EraseChip(void)
{
    /*!< Send write enable instruction */
    W25Qxx_WriteEnable();

    /*!< Bulk Erase */
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send Bulk Erase instruction  */
    sFLASH_SendByte(W25Q_ChipErase);
    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    W25Qxx_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /*!< Enable the write access to the FLASH */
    W25Qxx_WriteEnable();

    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send "Write to Memory " instruction */
    sFLASH_SendByte(W25Q_PageProgram);
    /*!< Send WriteAddr high nibble address byte to write to */
    sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*!< Send WriteAddr medium nibble address byte to write to */
    sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*!< Send WriteAddr low nibble address byte to write to */
    sFLASH_SendByte(WriteAddr & 0xFF);

    /*!< while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /*!< Send the current byte */
        sFLASH_SendByte(*pBuffer);
        /*!< Point on the next byte to be written */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    W25Qxx_WaitForWriteEnd();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % sFLASH_PAGESIZE;
    count = sFLASH_PAGESIZE - Addr;
    NumOfPage =  NumByteToWrite / sFLASH_PAGESIZE;
    NumOfSingle = NumByteToWrite % sFLASH_PAGESIZE;

    if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
        {
            W25Qxx_WritePage(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /*!< NumByteToWrite > sFLASH_PAGESIZE */
        {
            while (NumOfPage--)
            {
                W25Qxx_WritePage(pBuffer, WriteAddr, sFLASH_PAGESIZE);
                WriteAddr +=  sFLASH_PAGESIZE;
                pBuffer += sFLASH_PAGESIZE;
            }

            W25Qxx_WritePage(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
        {
            if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
            {
                temp = NumOfSingle - count;

                W25Qxx_WritePage(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                W25Qxx_WritePage(pBuffer, WriteAddr, temp);
            }
            else
            {
                W25Qxx_WritePage(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /*!< NumByteToWrite > sFLASH_PAGESIZE */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / sFLASH_PAGESIZE;
            NumOfSingle = NumByteToWrite % sFLASH_PAGESIZE;

            W25Qxx_WritePage(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                W25Qxx_WritePage(pBuffer, WriteAddr, sFLASH_PAGESIZE);
                WriteAddr +=  sFLASH_PAGESIZE;
                pBuffer += sFLASH_PAGESIZE;
            }

            if (NumOfSingle != 0)
            {
                W25Qxx_WritePage(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Read from Memory " instruction */
    sFLASH_SendByte(W25Q_ReadData);

    /*!< Send ReadAddr high nibble address byte to read from */
    sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /*!< Send ReadAddr medium nibble address byte to read from */
    sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /*!< Send ReadAddr low nibble address byte to read from */
    sFLASH_SendByte(ReadAddr & 0xFF);

    while (NumByteToRead--) /*!< while there is data to be read */
    {
        /*!< Read a byte from the FLASH */
        *pBuffer = sFLASH_SendByte(W25Q_DUMMY_BYTE);
        /*!< Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
