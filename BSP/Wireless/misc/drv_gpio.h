
#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include "HAL_gpio.h"
#include "drv_timer.h"

/* Exported macro --------------------------------------------- */
#define _AC_BUZZER

/*---------------------------------------------------------------
	U S E R 	G P I O 	C O N F I G 	M A C R O
----------------------------------------------------------------*/
/* LED_G PIN = PB8 */
#define _LEDG_PORT      GPIOB
#define _LEDG_PIN       GPIO_Pin_8
#define LED_G_ON        GPIO_SetBits(_LEDG_PORT, _LEDG_PIN);
#define LED_G_OFF       GPIO_ResetBits(_LEDG_PORT, _LEDG_PIN);
    
/* LED_R PIN = PB9 */
#define _LEDR_PORT      GPIOB
#define _LEDR_PIN       GPIO_Pin_9
#define LED_R_ON        GPIO_SetBits(_LEDR_PORT, _LEDR_PIN);
#define LED_R_OFF       GPIO_ResetBits(_LEDR_PORT, _LEDR_PIN);

/* BUZZER PIN = PB4 */
#define _BUZZ_PORT      GPIOB
#define _BUZZ_PIN       GPIO_Pin_4
#ifdef _AC_BUZZER
#define BUZZER_ON       PWM_Enable();
#define BUZZER_OFF      PWM_Disable();
#else
#define BUZZER_ON       GPIO_SetBits(_BUZZ_PORT, _BUZZ_PIN);
#define BUZZER_OFF      GPIO_ResetBits(_BUZZ_PORT, _BUZZ_PIN);
#endif /* _AC_BUZZER */
#define BEEP_OFF        GPIO_ResetBits(_BUZZ_PORT, _BUZZ_PIN);

/* TRIG_KEY PIN = PA0 */
#define _KEY_PORT       GPIOA
#define _KEY_PIN        GPIO_Pin_0
#define KEY_VAL         GPIO_ReadInputDataBit(_KEY_PORT, _KEY_PIN)

/* TRIG_CCD PIN = PB15 */
#define _CCD_PORT       GPIOB
#define _CCD_PIN        GPIO_Pin_15
#define TRIG_CCD_L      GPIO_ResetBits(_CCD_PORT, _CCD_PIN);
#define TRIG_CCD_H      GPIO_SetBits(_CCD_PORT, _CCD_PIN);

/* POWER PIN = PA8 */
#define _PWR_PORT       GPIOA
#define _PWR_PIN        GPIO_Pin_8
#define PWR_OUT_H		GPIO_SetBits(_PWR_PORT, _PWR_PIN);
#define PWR_OUT_L       GPIO_ResetBits(_PWR_PORT, _PWR_PIN);

/* POWER_CCD PIN = PC15 */
#define _PWR_CCD_PORT   GPIOC
#define _PWR_CCD_PIN    GPIO_Pin_15
#define PWR_CCD_ON		GPIO_SetBits(_PWR_CCD_PORT, _PWR_CCD_PIN);
#define PWR_CCD_OFF     GPIO_ResetBits(_PWR_CCD_PORT, _PWR_CCD_PIN);

//--------------------------------------------------------------------------
//              L A S E R   D E C O D E     D E F I N E
//--------------------------------------------------------------------------
/* LASER PIN = PB11 */
#define _LWS_PORT       GPIOB
#define _LWS_PIN        GPIO_Pin_11
#define LWS_ON          GPIO_ResetBits(_LWS_PORT, _LWS_PIN);
#define LWS_OFF         GPIO_SetBits(_LWS_PORT, _LWS_PIN);

/* CLK PIN = PB2 */
#define _CLK_PORT       GPIOB
#define _CLK_PIN        GPIO_Pin_2
#define CLK_VAL         GPIO_ReadInputDataBit(_CLK_PORT, _CLK_PIN)

/* DATA PIN = PB10 */
#define _DATA_PORT      GPIOB
#define _DATA_PIN       GPIO_Pin_10
#define DATA_VAL        GPIO_ReadInputDataBit(_DATA_PORT, _DATA_PIN)

//--------------------------------------------------------------------------
//              E E P R O M     D E F I N E
//--------------------------------------------------------------------------
/* I2C SDA PIN = PB7 */
#define _SDA_PORT       GPIOB
#define _SDA_PIN        GPIO_Pin_7
#define I2C_SDA_INPUT   {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (u32)8<<28;}
#define I2C_SDA_OUTPUT  {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (u32)3<<28;}

#define I2C_SDA_OUT_1   GPIO_SetBits(_SDA_PORT, _SDA_PIN);
#define I2C_SDA_OUT_0   GPIO_ResetBits(_SDA_PORT, _SDA_PIN);
#define I2C_SDA_IN      GPIO_ReadInputDataBit(_SDA_PORT, _SDA_PIN)

/* I2C SCL PIN = PB6 */
#define _SCL_PORT       GPIOB
#define _SCL_PIN        GPIO_Pin_6
#define I2C_SCL_INPUT   {GPIOB->CRL &= 0XF0FFFFFF;GPIOB->CRL |= (u32)8<<24;}
#define I2C_SCL_OUTPUT  {GPIOB->CRL &= 0XF0FFFFFF;GPIOB->CRL |= (u32)3<<24;}

#define I2C_SCL_OUT_1   GPIO_SetBits(_SCL_PORT, _SCL_PIN);
#define I2C_SCL_OUT_0   GPIO_ResetBits(_SCL_PORT, _SCL_PIN);
#define I2C_SCL_IN      GPIO_ReadInputDataBit(_SCL_PORT, _SCL_PIN)

//--------------------------------------------------------------------------
//              2.4 G       R F     D E F I N E
//--------------------------------------------------------------------------
/* SPI_SEL PIN = PB12 */
#define _SEL_PORT      	GPIOB
#define _SEL_PIN       	GPIO_Pin_12
#define SEL_HIGH		GPIO_SetBits(_SEL_PORT, _SEL_PIN);
#define SEL_LOW        	GPIO_ResetBits(_SEL_PORT, _SEL_PIN);

/* SPI_SCK PIN = PB13 */
#define _SCK_PORT      	GPIOB
#define _SCK_PIN       	GPIO_Pin_13
#define SCK_HIGH		GPIO_SetBits(_SCK_PORT, _SCK_PIN);
#define SCK_LOW        	GPIO_ResetBits(_SCK_PORT, _SCK_PIN);

/* SPI_DAT PIN = PB14 */
#define _DAT_PORT       GPIOB
#define _DAT_PIN        GPIO_Pin_14
#define SPI_DAT_INPUT   {GPIOB->CRH &= 0XF0FFFFFF; GPIOB->CRH |= (u32)8<<24;}
#define SPI_DAT_OUTPUT  {GPIOB->CRH &= 0XF0FFFFFF; GPIOB->CRH |= (u32)3<<24;}

#define SPI_DAT_OUT_1   GPIO_SetBits(_DAT_PORT, _DAT_PIN);
#define SPI_DAT_OUT_0   GPIO_ResetBits(_DAT_PORT, _DAT_PIN);
#define SPI_DAT_IN      GPIO_ReadInputDataBit(_DAT_PORT, _DAT_PIN)

//--------------------------------------------------------------------------
//              N O R       F L A S H       D E F I N E
//--------------------------------------------------------------------------
/* SPI_SEL PIN = PB0 */
#define _SEL1_PORT      GPIOB
#define _SEL1_PIN       GPIO_Pin_0
#define SEL1_HIGH		GPIO_SetBits(_SEL1_PORT, _SEL1_PIN);
#define SEL1_LOW        GPIO_ResetBits(_SEL1_PORT, _SEL1_PIN);

/* SPI_SCK PIN = PA5 */
#define _SCK1_PORT      GPIOA
#define _SCK1_PIN       GPIO_Pin_5
#define SCK1_HIGH		GPIO_SetBits(_SCK1_PORT, _SCK1_PIN);
#define SCK1_LOW        GPIO_ResetBits(_SCK1_PORT, _SCK1_PIN);

/* SPI_MISO PIN = PA6 */
#define _MISO1_PORT     GPIOA
#define _MISO1_PIN      GPIO_Pin_6
#define MISO1       	GPIO_ReadInputDataBit(_MISO1_PORT, _MISO1_PIN)

/* SPI_MOSI PIN = PA7 */
#define _MOSI1_PORT     GPIOA
#define _MOSI1_PIN      GPIO_Pin_7
#define MOSI1_HIGH		GPIO_SetBits(_MOSI1_PORT, _MOSI1_PIN);
#define MOSI1_LOW       GPIO_ResetBits(_MOSI1_PORT, _MOSI1_PIN);

//--------------------------------------------------------------------------
//              B T     M O D U L E     D E F I N E
//--------------------------------------------------------------------------
#define _BT_RSSI_PORT   GPIOC
#define _BT_RSSI_PIN    GPIO_Pin_13
#define BT_RSSI_IN      GPIO_ReadInputDataBit(_BT_RSSI_PORT, _BT_RSSI_PIN)

#define _BT_DISC_PORT   GPIOB
#define _BT_DISC_PIN    GPIO_Pin_5
#define BT_DISC_H       GPIO_SetBits(_BT_DISC_PORT, _BT_DISC_PIN);
#define BT_DISC_L       GPIO_ResetBits(_BT_DISC_PORT, _BT_DISC_PIN);

#define _BT_LINK_PORT   GPIOC
#define _BT_LINK_PIN    GPIO_Pin_14
#define BT_LINK_IN      GPIO_ReadInputDataBit(_BT_LINK_PORT, _BT_LINK_PIN)

/* Exported functions ------------------------------------------ */
void GPIO_Configuration(void);

#endif /* end of _DRV_GPIO_H_ */

