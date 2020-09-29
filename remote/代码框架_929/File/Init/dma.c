#include "stm32f4xx.h"
#include "dma.h"

uint8_t rxbuf[3];

void dma_usart3_init(void)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	//���ô���1����DMA
	DMA_DeInit(DMA1_Stream1);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rxbuf;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = 3;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream
	
	DMA_Cmd(DMA1_Stream1, ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
} 

void set_dma_usart3_rx_ndtr(void)
{
	DMA_Cmd(DMA1_Stream1, DISABLE);                      //�ر�DMA���� 
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE){}	//ȷ��DMA���Ա�����  
	DMA_SetCurrDataCounter(DMA1_Stream1, 3);          //���ݴ�����  
	DMA_Cmd(DMA1_Stream1, ENABLE);     
}
