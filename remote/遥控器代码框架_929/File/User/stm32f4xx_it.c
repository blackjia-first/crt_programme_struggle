/*******************************************************************************************************************
@@���ھ���Ӫ��Ӫ��Ŀ�жϺ���
	@<1>
	@<2>
	@<3>

*******************************************************************************************************************/
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
#include "main.h"
#include "dma.h"
#include "remoter.h"

/*************************************************************************
�� �� ����EXTI3_IRQHandler
�������ܣ��ⲿ�жϷ�����
��    ע��
*************************************************************************/
void EXTI3_IRQHandler(void)
{
	
	EXTI_ClearITPendingBit(EXTI_Line3);//���LINE3�ϵ��жϱ�־λ 
}


/*************************************************************************
�� �� ����EXTI4_IRQHandler
�������ܣ��ⲿ�жϷ�����
��    ע��
*************************************************************************/
void EXTI4_IRQHandler(void)
{
	
	EXTI_ClearITPendingBit(EXTI_Line4);//���LINE2�ϵ��жϱ�־λ 
}


/*************************************************************************
�� �� ����USART1_IRQHandler
�������ܣ������жϷ�����
��    ע��
*************************************************************************/
void USART1_IRQHandler(void) //����1�жϷ������
{
	
}

#ifdef REMOTER

void USART3_IRQHandler(void) //����1�жϷ������
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //�����ж�
	{
		USART3->SR;
		USART3->DR;
		
		if(DMA1_Stream1->NDTR == 3)
		{
			if(rxbuf[0] == 0xAB && rxbuf[2] == 0xCD)
			{
				switch(rxbuf[1])
				{
				case 0x01:
					motor1 = STILL1;
					break;
				case 0x02:
					motor1 = UP;
					break;
				case 0x03:
					motor1 = DOWN;
					break;
				case 0x04:
					motor2 = STILL2;
					break;
				case 0x05:
					motor2 = LEFT;
					break;
				case 0x06:
					motor2 = RIGHT;
					break;
				case 0x07:
					cylinder12 = CLOSE;
					break;
				case 0x08:
					cylinder12 = OPEN;
					break;
				case 0x09:
					cylinder3 = CLOSE;
					break;
				case 0x10:
					cylinder3 = OPEN;
					break;		
				}
			}
			else
			{
				//NONE
			}
		}
		else
		{
			//���������������
			//usart_send_string(USART1, text);
			//set_dma_rx_ndtr();
		}
	}		
}

#endif

/*************************************************************************
�� �� ����CAN2_TX_IRQHandler
�������ܣ�CAN�����жϷ�����
��    ע��
*************************************************************************/
void CAN2_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
    }
}


/*************************************************************************
�� �� ����CAN2_RX0_IRQHandler
�������ܣ�CAN�����жϷ�����
��    ע��
*************************************************************************/
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
				get_encoder(&encoder, &CAN2_RxMsg);
				break;
			default:
				break;
		}
	}
}


/*************************************************************************
�� �� ����TIM2_IRQHandler
�������ܣ�CAN�����жϷ�����
��    ע��
*************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//���TIMx���жϴ�����λ
		LED_1_TOGGLE();
		/***PID����***/
		//1��λ�û�
		//2�������
		//3�����͵�������ָ��
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}


