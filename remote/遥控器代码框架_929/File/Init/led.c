/*************************************************************************
函 数 名：led_init
函数功能：led初始化函数

*************************************************************************/
#include "stm32f4xx.h" 
#include "led.h"

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

	LED_1_OFF();
	LED_2_OFF();

}
