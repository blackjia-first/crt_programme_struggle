/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <math.h>

#include "stm32f4xx_it.h"

#include "key.h"
#include "delay.h"
#include "usart.h"
#include "dma.h"
#include "can.h"
#include "led.h"
#include "main.h"
#include "task.h"


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */



void SysTick_Handler(void)
{
		
}

//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(10);

	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))
	{
		key_status.key1 = ON;
		key_status.key0 = OFF;
		key_status.enter_key0_from_key1 = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);//清除LINE2上的中断标志位 
}

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);

	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3))
	{

		key_status.key0 = ON;
		if(key_status.key1 == ON)
		{
			key_status.key1 = OFF;
			key_status.enter_key0_from_key1 = 1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);//清除LINE3上的中断标志位 
}

char text[] = {"receive incorrect data\r\n"};

void USART1_IRQHandler(void) //串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //接收中断
	{
		USART1->SR;
		USART1->DR;
		
		if(DMA2_Stream2->NDTR == 8)
		{
			if(rxbuf[0] == 0x55 && rxbuf[1] == 0x11 && rxbuf[7] == 0xAA)
			{
				set_dma_tx_ndtr();
			}
			else
			{
				usart_send_string(USART1, text);
			}
		}
		else
		{
			usart_send_string(USART1, text);
			set_dma_rx_ndtr();
		}
	}		
}

void CAN2_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
    }
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
				get_encoder(&encoder, &CAN2_RxMsg);
				break;
			default:
				break;
		}
	}
}

uint32_t times = 0;
fp64 sin_y;

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		sin_y = sin(0.5f * 3.14f / 1000.0f * times);//4*3.14*times/1000
		set = sin_y * 8192 * encoder.reduction_ratio;
		times++;
		if(times == 4000)
		{
			times = 0;
		}	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

bool flag = 0;

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
//		if(!flag)
//		{
//			led1_on();
//			flag = 1;
//		}
//		else
//		{
//			led1_off();
//			flag = 0;
//		}
		set += 155648;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
