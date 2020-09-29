#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f4xx.h"

extern void delay_init(uint8_t SYSCLK);
//extern void systick_init(void);

extern void delay_us(uint32_t nus);
extern void delay_ms(uint16_t nms);

#endif
