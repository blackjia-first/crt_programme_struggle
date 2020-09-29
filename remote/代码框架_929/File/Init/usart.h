#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"	
#include "stm32f4xx_conf.h"

extern void usart1_init(uint32_t bound);
extern void usart3_init(uint32_t bound);
extern void data_handle(uint8_t data);
extern void usart_send_string(USART_TypeDef * pUSARTx, char *str);
extern void uart_send_byte(USART_TypeDef * pUSARTx, uint8_t ch);

#endif

