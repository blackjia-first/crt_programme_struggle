#include "stm32f4xx.h"
#include "dma.h"

uint8_t txbuf[] = {"receive correct data!(send by dma)\r\n"};
uint8_t rxbuf[8];

void dma_init(void)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	//���ô���1����DMA
	DMA_DeInit(DMA2_Stream7);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txbuf;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = 0;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);//��ʼ��DMA Stream
	
	DMA_Cmd(DMA2_Stream7, DISABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	//���ô���1����DMA
	DMA_DeInit(DMA2_Stream2);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rxbuf;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = 8;//���ݴ����� 
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
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);//��ʼ��DMA Stream
	
	DMA_Cmd(DMA2_Stream2, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
} 

void set_dma_tx_ndtr(void)
{
	DMA_Cmd(DMA2_Stream7, DISABLE);                      //�ر�DMA���� 
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}	//ȷ��DMA���Ա�����  
	DMA_SetCurrDataCounter(DMA2_Stream7, sizeof(txbuf));          //���ݴ�����  
	DMA_Cmd(DMA2_Stream7, ENABLE);                      //����DMA���� 
}	  

void set_dma_rx_ndtr(void)
{
	DMA_Cmd(DMA2_Stream2, DISABLE);                      //�ر�DMA���� 
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE){}	//ȷ��DMA���Ա�����  
	DMA_SetCurrDataCounter(DMA2_Stream2, 8);          //���ݴ�����  
	DMA_Cmd(DMA2_Stream2, ENABLE);     
}
