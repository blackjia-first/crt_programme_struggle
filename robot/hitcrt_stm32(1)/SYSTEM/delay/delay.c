#include <stm32f4xx.h>
#include "stm32f4xx.h" 
#include "delay.h"

static uint8_t fac_us=0;
static uint16_t fac_ms=0;

void delay_init(uint8_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //设置SysTick时钟源为系统时钟的8分频
	fac_us = SYSCLK/8;                                      //计数器SYSCLK/8次计数的时间为1us
	fac_ms = (uint16_t)fac_us*1000;                         //计数器SYSCLK/8*1000次计数的时间为1ms
}

//别用systick中断了，systick用来生成delay函数就可以了，需要中断定时时可以使用别的定时器
//void systick_init(void)
//{
//	if (SysTick_Config(SystemCoreClock / 1000))	//1ms中断一次
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//}

void delay_xms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD = (uint32_t)nms*fac_ms;       //SysTick定时器重新装载的数值
	SysTick->VAL = 0x00;                        //清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //使能SysTick定时器
	do
	{
		temp = SysTick->CTRL;
	}
	while(!(temp&(1<<16)));                     //通过检测COUNTFLAG标志位判断计数器是否清零   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //关闭SysTick定时器
	SysTick->VAL = 0X00;                        //清空计数器	  	    
} 

void delay_ms(uint16_t nms)
{	 	 
	uint8_t repeat = nms / 540;
	uint16_t remain = nms % 540;
	//SysTick重装载数值寄存器只有24位，故一次延时不能超过798915微秒，通过调用delay_xms函数解决了这一问题
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
		SysTick->LOAD = nus*fac_us;                 //SysTick定时器重新装载的数值	 
		SysTick->VAL = 0x00;                        //清空计数器
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //使能SysTick定时器
		do
		{
			temp = SysTick->CTRL;
		}
		while(!(temp&(1<<16)));                     //通过检测COUNTFLAG标志位判断计数器是否清零   
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //关闭SysTick定时器
		SysTick->VAL = 0X00;                        //清空计数器	
	}		
}			 
