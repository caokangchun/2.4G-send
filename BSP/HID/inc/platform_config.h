/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */

/* Define the STM32F10x hardware depending on the used evaluation board */
  #define USB_DISCONNECT                      GPIOA  
  #define USB_DISCONNECT_PIN                  GPIO_Pin_11|GPIO_Pin_12
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA 

  #define RCC_APB2Periph_GPIO_KEY             RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_GPIO_TAMPER          RCC_APB2Periph_GPIOA
  #define RCC_APB2Periph_GPIO_IOAIN           RCC_APB2Periph_GPIOA
  #define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD

  #define GPIO_KEY                            GPIOC
  #define GPIO_TAMPER                         GPIOA
  #define GPIO_IOAIN                          GPIOA
  #define GPIO_LED1                           GPIOA
	#define GPIO_LED2                           GPIOD

  #define GPIO_KEY1_PIN                       GPIO_Pin_5   /* PC.05 */
	#define GPIO_KEY2_PIN                       GPIO_Pin_1   /* PC.01 */
  #define GPIO_TAMPER_PIN                     GPIO_Pin_0   /* PA.00 */
  #define GPIO_IOAIN_PIN                      GPIO_Pin_1   /* PA.01 */
  
  #define GPIO_LED1_PIN                       GPIO_Pin_8   /* PA.08 */
  #define GPIO_LED2_PIN                       GPIO_Pin_2   /* PC.07 */ 

  #define GPIO_KEY1_PORTSOURCE                GPIO_PortSourceGPIOC
  #define GPIO_KEY1_PINSOURCE                 GPIO_PinSource5
  #define GPIO_KEY1_EXTI_Line                 EXTI_Line5
	
	#define GPIO_KEY2_PORTSOURCE                GPIO_PortSourceGPIOC
  #define GPIO_KEY2_PINSOURCE                 GPIO_PinSource1
  #define GPIO_KEY2_EXTI_Line                 EXTI_Line1

  #define GPIO_TAMPER_PORTSOURCE              GPIO_PortSourceGPIOA
  #define GPIO_TAMPER_PINSOURCE               GPIO_PinSource0
  #define GPIO_TAMPER_EXTI_Line               EXTI_Line0

#define ADC_AIN_CHANNEL                     ADC_Channel_1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

