#include "can.h" 

encoder_t encoder = {.sum_value = 0, .raw_value = 0, .speed = 0, .reduction_ratio = 19};//ע�⿴�õĵ�����ٱ��Ƕ���

unsigned int g_uiAirValve;	     //�����й�ȫ�ֱ���

void can2_init(void)
{
	GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;
    CAN_InitTypeDef        can2;
    CAN_FilterInitTypeDef  can2_filter;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��*ע��*������ʹ��can2,����ʹ��can1��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN2ʱ��,42MHz

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
    GPIO_Init(GPIOB, &gpio);//�����趨������ʼ��GPIOA

    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�   1101
    nvic.NVIC_IRQChannelSubPriority = 2;//�����ȼ�
    nvic.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&nvic);//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
    nvic.NVIC_IRQChannelSubPriority = 2;//�����ȼ�
    nvic.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&nvic);//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

    /****************************************CAN��Ԫ��ʼ��*****************************************/
    CAN_DeInit(CAN2);
    CAN_StructInit(&can2);
	/*CAN�����ƼĴ�������CAN_MCR��*/
    can2.CAN_TTCM = DISABLE;//ʱ�䴥��ͨ��ģʽ
    can2.CAN_ABOM = DISABLE;//�Զ������߹رչ���
    can2.CAN_AWUM = DISABLE;//�Զ�����ģʽ    
    can2.CAN_NART = DISABLE;//��ֹ�Զ��ط���    
    can2.CAN_RFLM = DISABLE;//����FIFO����ģʽ    
    can2.CAN_TXFP = ENABLE;//����FIFIO���ȼ�
	/*CANλʱ��Ĵ�����CAN_BTR��*/
	/*CAN1������=42MHz/3/(9+4+1)=1MHz*/     
    can2.CAN_Mode = CAN_Mode_Normal;//��ͨģʽ 
    can2.CAN_SJW  = CAN_SJW_1tq;//����ͬ����Ծ���
    can2.CAN_BS1 = CAN_BS1_9tq;//ʱ���1
    can2.CAN_BS2 = CAN_BS2_4tq;//ʱ���2
    can2.CAN_Prescaler = 3;//��Ƶϵ��
    CAN_Init(CAN2, &can2);//����ָ���Ĳ�����ʼ��CAN�Ĵ���
	/*CAN�������Ĵ���*/   
    can2_filter.CAN_FilterNumber=14;//������14
    can2_filter.CAN_FilterMode=CAN_FilterMode_IdMask;//��ʶ��������Ϊ����ģʽ
    can2_filter.CAN_FilterScale=CAN_FilterScale_32bit;//һ��32λ��ʶ��������
    can2_filter.CAN_FilterIdHigh=0x0000;//32λ��ʶ���������ĸ�16λ
    can2_filter.CAN_FilterIdLow=0x0000;//32λ��ʶ���������ĵ�16λ
    can2_filter.CAN_FilterMaskIdHigh=0x0000;//�����������16λ
    can2_filter.CAN_FilterMaskIdLow=0x0000;//�����������16λ
    can2_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
    can2_filter.CAN_FilterActivation=ENABLE;//���������
    CAN_FilterInit(&can2_filter);//����ָ���Ĳ�����ʼ��CAN_Filter�Ĵ���   
	/*CAN�ж�ʹ�ܼĴ�����CAN_IER��*/
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//�����ж�:FIFO 0��Ϣ�����ж�
    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);//�����жϣ�����������ж�
}




