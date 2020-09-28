#include <math.h>
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
#include "led.h"
#include "main.h"

/*************************************************************************
函 数 名：EXTI3_IRQHandler
函数功能：外部中断服务函数
备    注：
*************************************************************************/
void EXTI3_IRQHandler(void)
{
	
	EXTI_ClearITPendingBit(EXTI_Line3);//清除LINE3上的中断标志位 
}


/*************************************************************************
函 数 名：EXTI4_IRQHandler
函数功能：外部中断服务函数
备    注：
*************************************************************************/
void EXTI4_IRQHandler(void)
{
	
	EXTI_ClearITPendingBit(EXTI_Line4);//清除LINE2上的中断标志位 
}


/*************************************************************************
函 数 名：USART1_IRQHandler
函数功能：串口中断服务函数
备    注：
*************************************************************************/
void USART1_IRQHandler(void) //串口1中断服务程序
{
//	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //接收中断
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
函 数 名：CAN2_TX_IRQHandler
函数功能：CAN接收中断服务函数
备    注：
*************************************************************************/
void CAN2_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
    }
}


/*************************************************************************
函 数 名：CAN2_RX0_IRQHandler
函数功能：CAN接收中断服务函数
备    注：
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
函 数 名：TIM2_IRQHandler
函数功能：CAN接收中断服务函数
备    注：
*************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除TIMx的中断待处理位
		LED_1_TOGGLE();
		/***PID控制***/
		//1、位置环
		//2、输出环
		//3、发送电流控制指令
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}


