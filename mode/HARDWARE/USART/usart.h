#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

enum
{
	FREE1,
	FREE2,
	LENGTH,
	DATA,
	TAIL
};

typedef struct
{
	uint8_t buf[500];
	uint8_t data_to_handle[50];
}uart1_t;

extern uart1_t uart1;
	
extern void uart_init(uint32_t bound);
extern void data_handle(uint8_t data);
extern void usart_send_string(USART_TypeDef * pUSARTx, char *str);

#endif

