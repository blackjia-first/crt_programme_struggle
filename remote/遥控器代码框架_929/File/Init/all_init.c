/*************************************************************************
函 数 名：all_init.h
函数功能：所有初始化函数

*************************************************************************/
#include "all_init.h"

void ALL_Iinitialization()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级(0-3共4级)，2位子优先级(0-3共4级)
	delay_init(168);
	led_init();
	can2_init();
	tim2_init(10-1,8400-1);//1ms
}

