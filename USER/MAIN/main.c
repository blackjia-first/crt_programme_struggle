#include "main.h"
#include "can.h"
#include "delay.h"
#include "task.h"
#include "pid.h"
#include "led.h"
#include "key.h"
#include "tim.h"

fp32 angle_pid[3] = {1.5f, 0.001f, 3.0f};//�����϶���Ҫ���޸�
fp32 speed_pid[3] = {2.0f, 0.01f, 5.0f};
fp32 pid_max_out = 4000;
fp32 pid_max_iout = 500;
int32_t ori, set;
bool motor_off = 0;

int main(void)
{
	//led_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	can2_init();
	tim3_init(10-1,8400-1);//1ms
	//tim2_init(10000-1,8400-1);
	pid_init(&motor_pid.motor_angle_pid[0], PID_POSITION, angle_pid, pid_max_out, pid_max_iout);
	pid_init(&motor_pid.motor_speed_pid[0], PID_POSITION, speed_pid, pid_max_out, pid_max_iout);
	
	while(1)
	{
		while(!encoder.sum_value)//ȷ���Ѿ��յ�������̵ķ���ֵ
		{
			ori = encoder.sum_value;
		}
		if(!motor_off)//��debug���޸�motor_off��ֵ���Թرյ��
		{
			pid_calc(&motor_pid.motor_angle_pid[0], encoder.sum_value , (fp32)ori+set);
			pid_calc(&motor_pid.motor_speed_pid[0], encoder.speed, motor_pid.motor_angle_pid[0].out);
			can2_send_current((int16_t)(motor_pid.motor_speed_pid[0].out));
		}
		else
		{
			can2_send_current(0);
		}
		delay_ms(1);//������ʱ�������������ݣ���ռ��CANͨ���������޷��������̷��ص�����
	}
}

