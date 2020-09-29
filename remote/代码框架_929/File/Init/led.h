#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx_gpio.h"
#define LED_GPIO_PORT GPIOE
#define LED_GPIO_PIN (GPIO_Pin_15 | GPIO_Pin_8)
#define LED_GPIO_CLK RCC_AHB1Periph_GPIOE

#define  LED_1_ON()      GPIOE->BSRRL = GPIO_Pin_15
#define  LED_1_OFF()     GPIOE->BSRRH = GPIO_Pin_15
#define  LED_1_TOGGLE()  GPIOE->ODR ^= GPIO_Pin_15

#define  LED_2_ON()      GPIOE->BSRRL = GPIO_Pin_8
#define  LED_2_OFF()     GPIOE->BSRRH = GPIO_Pin_8
#define  LED_2_TOGGLE()  GPIOE->ODR ^= GPIO_Pin_8

extern void led_init(void);

#endif
