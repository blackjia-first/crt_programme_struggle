#ifndef __LED_H__
#define __LED_H__

#define LED_GPIO_PORT GPIOA
#define LED_GPIO_PIN (GPIO_Pin_6 | GPIO_Pin_7)
#define LED_GPIO_CLK RCC_AHB1Periph_GPIOA

extern void led_init(void);
extern void led1_on(void);
extern void led1_off(void);
extern void led2_on(void);
extern void led2_off(void);
extern void led_flashing(int fre);

#endif
