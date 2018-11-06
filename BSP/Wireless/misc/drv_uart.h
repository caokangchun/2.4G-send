
#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#include "HAL_uart.h"
#include "HAL_gpio.h"
#include <stdio.h>

void UART_Configuration(UART_TypeDef* UARTx, uint32_t baudrate);
void UART_IRQConfig(UART_TypeDef* UARTx, uint8_t prio);
void UART_SendByte(UART_TypeDef* UARTx, uint8_t data);
void UART_SendDatas(UART_TypeDef* UARTx, uint8_t *pdata, uint16_t len);

#endif /* end of _DRV_UART_H_ */

