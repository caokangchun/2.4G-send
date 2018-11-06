
#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include "HAL_spi.h"

#define RCC_SPI1_IO         RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
#define RCC_SPI2_IO         RCC_APB2Periph_GPIOB

#define sFLASH_SPI          SPI1
#define SPI1_NSS_PORT       GPIOB
#define SPI1_NSS_PIN        GPIO_Pin_0
#define SPI1_PORT           GPIOA
#define SPI1_SCK_PIN        GPIO_Pin_5
#define SPI1_MISO_PIN       GPIO_Pin_6
#define SPI1_MOSI_PIN       GPIO_Pin_7

#define sRF_SPI             SPI2
#define SPI2_NSS_PORT       GPIOB
#define SPI2_NSS_PIN        GPIO_Pin_12
#define SPI2_PORT           GPIOB
#define SPI2_SCK_PIN        GPIO_Pin_13
#define SPI2_MISO_PIN       GPIO_Pin_14
#define SPI2_MOSI_PIN       GPIO_Pin_15

void    sFLASH_Configuration(void);
uint8_t sFLASH_SendByte(uint8_t byte);
void    sRF_Configuration(void);
uint8_t sRF_SendByte(uint8_t byte);

#endif /* end of _DRV_SPI_H_ */

