#include "stm32f4xx.h"
#include "dma.h"

uint8_t rxbuf[3];

void dma_usart3_init(void)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	//配置串口1接收DMA
	DMA_DeInit(DMA1_Stream1);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rxbuf;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = 3;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用循环模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);//初始化DMA Stream
	
	DMA_Cmd(DMA1_Stream1, ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
} 

void set_dma_usart3_rx_ndtr(void)
{
	DMA_Cmd(DMA1_Stream1, DISABLE);                      //关闭DMA传输 
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE){}	//确保DMA可以被设置  
	DMA_SetCurrDataCounter(DMA1_Stream1, 3);          //数据传输量  
	DMA_Cmd(DMA1_Stream1, ENABLE);     
}
