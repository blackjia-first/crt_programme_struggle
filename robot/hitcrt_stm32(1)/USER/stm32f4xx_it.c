#include <math.h>
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
#include "led.h"
#include "main.h"

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
//	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //�����ж�
//	{
//		USART1->SR;
//		USART1->DR;
//		
//		if(DMA2_Stream2->NDTR == 8)
//		{
//			if(rxbuf[0] == 0x55 && rxbuf[1] == 0x11 && rxbuf[7] == 0xAA)
//			{
//				set_dma_tx_ndtr();
//			}
//			else
//			{
//				usart_send_string(USART1, text);
//			}
//		}
//		else
//		{
//			usart_send_string(USART1, text);
//			set_dma_rx_ndtr();
//		}
//	}		
}


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


