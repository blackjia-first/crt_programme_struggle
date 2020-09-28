#include "stm32f4xx_it.h"
#include "PID.h"
#include "can.h"

extern ST_ENCODER Encoder;
s16 Speed;
u32 Degree;
float degree;

int n;
void CAN2_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
    }
}

float Degree_Process(float position,int a)
{
  float degree;
	position=Encoder.SumValue;
	degree=360*position/8192/a;
	
	
	return degree;

}

void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg CAN2_RxMsg;
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);		
		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RxMsg);
		switch(CAN2_RxMsg.StdId)
		{
			case 0x201:
				Degree = GetEncoderNumber(&CAN2_RxMsg);
			 //编码器溢出处理
				Abs_Encoder_Process(&Encoder, GetEncoderNumber(&CAN2_RxMsg));
				//PID获取速度反馈
				SetFeedBack(&speed_pid_1,GetSpeed(&CAN2_RxMsg)/27); 
								
				degree=Degree_Process(Degree,27);
                
				SetFeedBack(&pos_pid_1,degree);
								
				break;
			case 0x203:
				Degree = GetEncoderNumber(&CAN2_RxMsg);
			  //编码器溢出处理
				Abs_Encoder_Process(&Encoder, GetEncoderNumber(&CAN2_RxMsg));
				//PID获取速度反馈
				SetFeedBack(&speed_pid_2,GetSpeed(&CAN2_RxMsg)/19);
			
       
								
				degree=Degree_Process(Degree,19);
                
				SetFeedBack(&pos_pid_2,degree);
			default:
				break;
		}
	}
}

void SysTick_Handler(void)
{
   n++;
}

void USART1_IRQHandler(void) //串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //空闲中断
	{
		USART1->SR;
		USART1->DR; //清除空闲中断的中断标志位
	}
}

void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}





