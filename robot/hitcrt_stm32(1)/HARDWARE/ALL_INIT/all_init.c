#include "all_init.h"

void ALL_Iinitialization()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ�(0-3��4��)��2λ�����ȼ�(0-3��4��)
	delay_init(168);
	led_init();
	can2_init();
	tim2_init(10-1,8400-1);//1ms
}
