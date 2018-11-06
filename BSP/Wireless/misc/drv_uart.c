
#include "drv_uart.h"
#include "BSPTimer.h"
#include "bluetooth.h"
	
/**
  * @brief  Configuare USART.
  * @param  None
  * @retval None
  */
void UART_Configuration(UART_TypeDef* UARTx, uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;

    if (UARTx == UART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if (UARTx == UART2)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    }
    else if (UARTx == UART3)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }

    if (UARTx == UART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    }
    else if (UARTx == UART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
    }
    else if (UARTx == UART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART3, ENABLE);
    }
    
    UART_InitStructure.UART_BaudRate =  baudrate;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_Init(UARTx, &UART_InitStructure);
    
    UART_ITConfig(UARTx, UART_IT_RXIEN, ENABLE);
    UART_ITConfig(UARTx, UART_IT_TXIEN, DISABLE);
    
    UART_IRQConfig(UARTx, 2);
    
    UART_Cmd(UARTx, ENABLE);
}

void UART_IRQConfig(UART_TypeDef* UARTx, uint8_t prio)
{
    NVIC_InitTypeDef nvic_uart;

    if (UARTx == UART1)
    {
        nvic_uart.NVIC_IRQChannel = UART1_IRQn;
    }
    else if (UARTx == UART2)
    {
        nvic_uart.NVIC_IRQChannel = UART2_IRQn;
    }
    else if (UARTx == UART3)
    {
        nvic_uart.NVIC_IRQChannel = UART3_IRQn;
    }

    nvic_uart.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_uart.NVIC_IRQChannelSubPriority = prio;
    nvic_uart.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&nvic_uart);
}

void UART_SendByte(UART_TypeDef* UARTx, uint8_t data)
{
	UART_SendData(UARTx, data);
	while(UART_GetFlagStatus(UARTx, UART_FLAG_TXEPT) == RESET);
}

void UART_SendDatas(UART_TypeDef* UARTx, uint8_t *pdata, uint16_t len)
{
	uint16_t i;

	for(i = 0; i < len; i++)
	{
		UART_SendByte(UARTx, pdata[i]);
		BSPTimerDelay(100);
//		bt.rx_tout = 0;
	}
}

//#define _DEBUG

#ifdef _DEBUG
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */

	/* e.g. write a character to the USART */
	UART_SendData(USART1, (uint8_t) ch);
	
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	
	return ch;
}
#endif /* _DEBUG */


