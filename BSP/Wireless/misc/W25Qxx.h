/**
  ******************************************************************************
  * @file    stm32_eval_spi_flash.h
  * @author  MCD Application Team
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file contains all the functions prototypes for the stm32_eval_spi_flash
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_SPI_FLASH_H
#define __STM32_SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "drv_spi.h"
#include "drv_gpio.h"

/**
  * @brief  W25Qxx SPI Flash supported commands
  */
#define W25Q_WriteEnable            0x06
#define W25Q_WriteDisable		    0x04
#define W25Q_ReadStatusReg1		    0x05
#define W25Q_ReadStatusReg2		    0x35
#define W25Q_WriteStatusReg		    0x01
#define W25Q_PageProgram		    0x02
#define W25Q_QuadPageProgram		0x32
#define W25Q_BlockErase64K			0xD8
#define W25Q_BlockErase32K			0x52
#define W25Q_SectorErase		    0x20
#define W25Q_ChipErase			    0xC7
#define W25Q_EraseSuspend			0x75
#define W25Q_EraseResume			0x7A
#define W25Q_PowerDown			    0xB9
#define W25Q_HighPerformMode		0xA3
#define W25Q_ContinuousRdMode		0xFF
#define W25Q_ReleasePowerDown	    0xAB
#define W25Q_ManufactDeviceID   	0x90
#define W25Q_ReadUniqueID			0x4B
#define W25Q_JedecID		        0x9F
#define W25Q_ReadData			    0x03
#define W25Q_FastReadData		    0x0B
#define W25Q_FastReadDualOut		0x3B
#define W25Q_FastReadDualIO		    0xBB
#define W25Q_FastReadQuadOut		0x6B
#define W25Q_FastReadQuadIO		    0xEB
#define W25Q_OctalWordRead		    0xE3

#define W25Q_DUMMY_BYTE             0xFF

#define sFLASH_WIP_FLAG             0x01  /*!< Write In Progress (WIP) flag */

#define sFLASH_PAGESIZE             0x100
#define sFLASH_W25Q80_ID            0xEF4014
#define sFLASH_W25Q16_ID            0xEF4015
#define sFLASH_W25Q32_ID            0xEF4016

//#define sFLASH_SendByte             SPI1_SendByte
/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()             SEL1_LOW
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()            SEL1_HIGH

/**
  * @brief  High layer functions
  */
uint32_t W25Qxx_Init(void);
uint32_t W25Qxx_ReadID(void);
void     W25Qxx_WriteEnable(void);
void     W25Qxx_WaitForWriteEnd(void);
void     W25Qxx_EraseSector(uint32_t SectorAddr);
void     W25Qxx_EraseBlock32K(uint32_t Block32Addr);
void     W25Qxx_EraseBlock64K(uint32_t Block64Addr);
void     W25Qxx_EraseChip(void);
void     W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void     W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void     W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

#endif /* __STM32_SPI_FLASH_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
