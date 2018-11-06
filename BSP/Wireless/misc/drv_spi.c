
#include "drv_spi.h"
#include "drv_gpio.h"

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    /*!< sFLASH_SPI Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_SPI1_IO | RCC_APB2Periph_SPI1, ENABLE);

    /*!< Configure sFLASH_SPI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_PORT, &GPIO_InitStructure);
    
    /*!< Configure sFLASH_SPI pins: MOSI */
    GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI1_PORT, &GPIO_InitStructure);
    
    /*!< Configure sFLASH_SPI pins: MISO */
    GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SPI1_PORT, &GPIO_InitStructure);
    
    /*!< Configure SPI1_NSS pin: sFLASH CS pin */
    GPIO_InitStructure.GPIO_Pin = SPI1_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI1_NSS_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(SPI1_NSS_PORT, SPI1_NSS_PIN); // NSS = 1
    GPIO_ResetBits(SPI1_PORT, SPI1_SCK_PIN);   // SCK = 0

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(sFLASH_SPI, &SPI_InitStructure);

    SPI_BiDirectionalLineConfig(sFLASH_SPI, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(sFLASH_SPI, SPI_Direction_Rx);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(sFLASH_SPI, ENABLE);
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sFLASH_SendByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  //while (SPI_GetFlagStatus(sFLASH_SPI, SPI_FLAG_TXEPT) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_SendData(sFLASH_SPI, byte);

  /*!< Wait to receive a byte */
  while (SPI_GetFlagStatus(sFLASH_SPI, SPI_FLAG_RXAVL) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_ReceiveData(sFLASH_SPI);
}

/**
  * @brief  Initializes the peripherals used by the SPI RF driver.
  * @param  None
  * @retval None
  */
void sRF_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	// SPI_SCK pin configuration
    GPIO_InitStructure.GPIO_Pin = _SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(_SCK_PORT, &GPIO_InitStructure);

	// SPI_SEL pin configuration
    GPIO_InitStructure.GPIO_Pin = _SEL_PIN;
    GPIO_Init(_SEL_PORT, &GPIO_InitStructure);

	SCK_LOW;
	SEL_HIGH;
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sRF_SendByte(uint8_t byte)
{
    uint8_t i, dat;
        
    for(i = 0; i < 8; i++)
    {
        SPI_DAT_OUTPUT;
        
        if(byte & 0x80) // send bit
        {
            SPI_DAT_OUT_1;
        }
        else
        {
            SPI_DAT_OUT_0;
        }
            
        byte <<= 1;     // next bit for Tx

        __NOP();__NOP();__NOP();__NOP();

        SCK_HIGH;       // pre edge

        SPI_DAT_INPUT;

        dat <<= 1;      // next bit for Rx
        
        if(SPI_DAT_IN != 0) // sample bit
        {
            dat |= 0x01;
        }
        else
        {
            dat &= 0xFE;
        }

        __NOP();__NOP();__NOP();__NOP();
        
        SCK_LOW;        // post edge
    }

    return dat;
}

