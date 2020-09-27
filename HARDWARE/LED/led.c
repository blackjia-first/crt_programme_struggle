#include "stm32f4xx.h" 
#include "led.h"
#include "delay.h"

void led1_on(void)
{
	GPIO_ResetBits(LED_GPIO_PORT, GPIO_Pin_6);
}

void led1_off(void)
{
	GPIO_SetBits(LED_GPIO_PORT, GPIO_Pin_6);
}

void led2_on(void)
{
	GPIO_ResetBits(LED_GPIO_PORT, GPIO_Pin_7);
}

void led2_off(void)
{
	GPIO_SetBits(LED_GPIO_PORT, GPIO_Pin_7);
}

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LED_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

	led1_off();
	led2_off();
}

void led_flashing(int fre)
{
	GPIO_SetBits(LED_GPIO_PORT, GPIO_Pin_7);
	delay_ms(fre);
	GPIO_ResetBits(LED_GPIO_PORT, GPIO_Pin_7);
	delay_ms(fre);
}
