/*************************************************************************
函 数 名：can2_init
函数功能：can2初始化函数

*************************************************************************/
#include "can.h" 

void can2_init(void)
{
	  GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;
    CAN_InitTypeDef        can2;
    CAN_FilterInitTypeDef  can2_filter;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PB端口时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟*注意*若单独使用can2,需先使能can1的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN2时钟,42MHz

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;//复用模式
    GPIO_Init(GPIOB, &gpio);//根据设定参数初始化GPIOA

    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级   1101
    nvic.NVIC_IRQChannelSubPriority = 2;//子优先级
    nvic.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    NVIC_Init(&nvic);//根据指定的参数初始化NVIC寄存器
    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
    nvic.NVIC_IRQChannelSubPriority = 2;//子优先级
    nvic.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    NVIC_Init(&nvic);//根据指定的参数初始化NVIC寄存器

    /****************************************CAN单元初始化*****************************************/
    CAN_DeInit(CAN2);
    CAN_StructInit(&can2);
	/*CAN主控制寄存器器（CAN_MCR）*/
    can2.CAN_TTCM = DISABLE;//时间触发通信模式
    can2.CAN_ABOM = DISABLE;//自动的总线关闭管理
    can2.CAN_AWUM = DISABLE;//自动唤醒模式    
    can2.CAN_NART = DISABLE;//禁止自动重发送    
    can2.CAN_RFLM = DISABLE;//接收FIFO锁定模式    
    can2.CAN_TXFP = ENABLE;//发送FIFIO优先级
	/*CAN位时序寄存器（CAN_BTR）*/
	/*CAN1波特率=42MHz/3/(9+4+1)=1MHz*/     
    can2.CAN_Mode = CAN_Mode_Normal;//普通模式 
    can2.CAN_SJW  = CAN_SJW_1tq;//重新同步跳跃宽度
    can2.CAN_BS1 = CAN_BS1_9tq;//时间段1
    can2.CAN_BS2 = CAN_BS2_4tq;//时间段2
    can2.CAN_Prescaler = 3;//分频系数
    CAN_Init(CAN2, &can2);//根据指定的参数初始化CAN寄存器
	/*CAN过滤器寄存器*/   
    can2_filter.CAN_FilterNumber=14;//过滤器14
    can2_filter.CAN_FilterMode=CAN_FilterMode_IdMask;//标识符过滤器为掩码模式
    can2_filter.CAN_FilterScale=CAN_FilterScale_32bit;//一个32位标识符过滤器
    can2_filter.CAN_FilterIdHigh=0x0000;//32位标识符过滤器的高16位
    can2_filter.CAN_FilterIdLow=0x0000;//32位标识符过滤器的低16位
    can2_filter.CAN_FilterMaskIdHigh=0x0000;//过滤器掩码高16位
    can2_filter.CAN_FilterMaskIdLow=0x0000;//过滤器掩码低16位
    can2_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
    can2_filter.CAN_FilterActivation=ENABLE;//激活过滤器
    CAN_FilterInit(&can2_filter);//根据指定的参数初始化CAN_Filter寄存器   
	/*CAN中断使能寄存器（CAN_IER）*/
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//接收中断:FIFO 0消息挂起中断
    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);//发送中断：发送邮箱空中断
}




