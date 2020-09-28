#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
//#include "stm32f4xx_can.h"
#include "can.h"
#include "PID.h"
//#include "math.h"

ST_ENCODER Encoder = {0, 0, 0, 8192, 0};




float PosDes_1= 0;
float SpeedDes_1= 0;
float PosDes_2 = 0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	CAN2_Configuration();
	pid_init(&speed_pid_1, 2000,0,0,0,5000);
  pid_init(&pos_pid_1, 2000,0,0,0,5000);
	pid_init(&speed_pid_2, 2000,0,0,0,5000);
  pid_init(&pos_pid_2, 2000,0,0,0,5000);
	
		
		
		while(1)
		{
//		SetDes(&pos_pid_1,PosDes_1);
//		Calculate(&pos_pid_1);
	 //SetDes(&speed_pid_1,pos_pid_1.m_Output);
			SetDes(&speed_pid_1,2000);
		Calculate(&speed_pid_1);
		CAN_SendCurrent(speed_pid_1.m_Output);
		delay_ms(1);
		}
	}






