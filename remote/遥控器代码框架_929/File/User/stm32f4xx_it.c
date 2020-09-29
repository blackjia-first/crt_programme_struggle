/*******************************************************************************************************************
@@暑期竞培营结营项目中断函数
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
	
}

#ifdef REMOTER

void USART3_IRQHandler(void) //串口1中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //接收中断
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
			//蓝牙传输的特殊性
			//usart_send_string(USART1, text);
			//set_dma_rx_ndtr();
		}
	}		
}

#endif

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


