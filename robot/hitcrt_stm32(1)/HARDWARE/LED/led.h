#ifndef __LED_H__
#define __LED_H__

#define LED_GPIO_PORT GPIOE
#define LED_GPIO_PIN (GPIO_Pin_15 | GPIO_Pin_8)
#define LED_GPIO_CLK RCC_AHB1Periph_GPIOE

#define  LED_1_ON()      GPIOA->BRR = GPIO_Pin_15
#define  LED_1_OFF()     GPIOA->BSRR = GPIO_Pin_15
#define  LED_1_TOGGLE()  GPIOA->ODR ^= GPIO_Pin_15

#define  LED_2_ON()      GPIOD->BRR = GPIO_Pin_8
#define  LED_2_OFF()     GPIOD->BSRR = GPIO_Pin_8
#define  LED_2_TOGGLE()  GPIOD->ODR ^= GPIO_Pin_8

extern void led_init(void);
extern void led1_on(void);
extern void led1_off(void);
extern void led2_on(void);
extern void led2_off(void);
extern void led_flashing(int fre);

#endif
