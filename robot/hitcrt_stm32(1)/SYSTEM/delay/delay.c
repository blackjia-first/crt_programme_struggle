#include <stm32f4xx.h>
#include "stm32f4xx.h" 
#include "delay.h"

static uint8_t fac_us=0;
static uint16_t fac_ms=0;

void delay_init(uint8_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //����SysTickʱ��ԴΪϵͳʱ�ӵ�8��Ƶ
	fac_us = SYSCLK/8;                                      //������SYSCLK/8�μ�����ʱ��Ϊ1us
	fac_ms = (uint16_t)fac_us*1000;                         //������SYSCLK/8*1000�μ�����ʱ��Ϊ1ms
}

//����systick�ж��ˣ�systick��������delay�����Ϳ����ˣ���Ҫ�ж϶�ʱʱ����ʹ�ñ�Ķ�ʱ��
//void systick_init(void)
//{
//	if (SysTick_Config(SystemCoreClock / 1000))	//1ms�ж�һ��
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//}

void delay_xms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD = (uint32_t)nms*fac_ms;       //SysTick��ʱ������װ�ص���ֵ
	SysTick->VAL = 0x00;                        //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //ʹ��SysTick��ʱ��
	do
	{
		temp = SysTick->CTRL;
	}
	while(!(temp&(1<<16)));                     //ͨ�����COUNTFLAG��־λ�жϼ������Ƿ�����   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //�ر�SysTick��ʱ��
	SysTick->VAL = 0X00;                        //��ռ�����	  	    
} 

void delay_ms(uint16_t nms)
{	 	 
	uint8_t repeat = nms / 540;
	uint16_t remain = nms % 540;
	//SysTick��װ����ֵ�Ĵ���ֻ��24λ����һ����ʱ���ܳ���798915΢�룬ͨ������delay_xms�����������һ����
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)
	{
		delay_xms(remain);
	}
} 

void delay_us(uint32_t nus)
{
	uint32_t temp, repeat, remain;

	if(nus > 700000)
	{
		repeat = nus / 700000;
		remain = nus % 700000;
		delay_ms(700 * repeat);
		if(remain) 
		{
			delay_us(remain);
		}
	}			
	else
	{
		SysTick->LOAD = nus*fac_us;                 //SysTick��ʱ������װ�ص���ֵ	 
		SysTick->VAL = 0x00;                        //��ռ�����
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //ʹ��SysTick��ʱ��
		do
		{
			temp = SysTick->CTRL;
		}
		while(!(temp&(1<<16)));                     //ͨ�����COUNTFLAG��־λ�жϼ������Ƿ�����   
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //�ر�SysTick��ʱ��
		SysTick->VAL = 0X00;                        //��ռ�����	
	}		
}			 
