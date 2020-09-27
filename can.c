#include "can.h"

CanRxMsg  CAN_Rece_Data;
CanTxMsg  CAN_Tran_Data;

//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 

//F4��ʱ��Ĭ��Ƶ��Ϊ��AHB=84M  APB1=42M

/*-------------------------------------------------------------------------------------------------
�� �� ����CAN1_Mode_Init
�������ܣ�����CAN1����ؼĴ���
��    ע�������ʡ�ģʽ���û��Լ�����
-------------------------------------------------------------------------------------------------*/
void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;
    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
		CAN_ITConfig (CAN1,CAN_IT_FMP0,ENABLE);
		CAN_ITConfig (CAN1,CAN_IT_TME,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}   
 
 
///*------------------------------------------------------------
//������:CAN_Configuration()
//��������: ��ʼ��ϵͳCAN���ߣ�CAN1&CAN2
//����: ��
//�������
//��ע:
//-------------------------------------------------------------*/
//void CAN_Configuration(void)
//{
//	/* Private variables ---------------------------------------------------------*/
//	CAN_InitTypeDef        CAN_InitStructure;
//	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	/* CAN GPIOs configuration **************************************************/
//	/* Connect CAN pins to AF9 */
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); 

//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2); 
//  
//	/* Configure CAN RX and TX pins */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_5 | GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	/* CAN configuration ********************************************************/  
//	/* Enable CAN clock */
//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
//	/* CAN register init */
//	CAN_DeInit(CAN1);
//	CAN_DeInit(CAN2);


//	/* CAN cell init */
//	CAN_InitStructure.CAN_TTCM = DISABLE;//�¼�����
//	CAN_InitStructure.CAN_ABOM = ENABLE;//�Զ����߹���
//	CAN_InitStructure.CAN_AWUM = ENABLE;//�Զ�����
//	CAN_InitStructure.CAN_NART = DISABLE;//�����ش�
//	CAN_InitStructure.CAN_RFLM = DISABLE;
//	CAN_InitStructure.CAN_TXFP = ENABLE;
//	CAN_InitStructure.CAN_Mode = CAN_Mode_Silent_LoopBack;
//	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
//	CAN_InitStructure.CAN_Prescaler = 3;//500K
//	CAN_Init(CAN2, &CAN_InitStructure);

//	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
//	CAN_InitStructure.CAN_Prescaler = 3;//1M
//	CAN_Init(CAN1, &CAN_InitStructure);
//	/* CAN filter init */
//	CAN_FilterInitStructure.CAN_FilterNumber = 0;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);
//	
//	CAN_FilterInitStructure.CAN_FilterNumber = 14;
//	CAN_FilterInit(&CAN_FilterInitStructure);
//	
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//�����ж�
//	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//�����ж�		
//}

/*-------------------------------------------------------------------------------------------------
�� �� ����CAN_SendData
�������ܣ�ͨ��can1��������
��    ע��
-------------------------------------------------------------------------------------------------*/
void CAN_SendData(u32 ID,s16 CURRENT_1,s16 CURRENT_2,s16 CURRENT_3,s16 CURRENT_4)
{
	CAN_Tran_Data.Data[0]= (u8)((CURRENT_1>>8)&0xFF);
	CAN_Tran_Data.Data[1]=(u8)(CURRENT_1&0xFF);
	CAN_Tran_Data.Data[2]= (u8)((CURRENT_2>>8)&0xFF);
	CAN_Tran_Data.Data[3]= (u8)(CURRENT_2&0xFF);
	CAN_Tran_Data.Data[4]=(u8)((CURRENT_3>>8)&0xFF);
	CAN_Tran_Data.Data[5]=(u8)(CURRENT_3&0xFF);
	CAN_Tran_Data.Data[6]=(u8)((CURRENT_4>>8)&0xFF);
	CAN_Tran_Data.Data[7]=(u8)(CURRENT_4&0xFF);
	CAN_Tran_Data.StdId=ID;
	CAN_Tran_Data.RTR = CAN_RTR_Data;//����֡
	CAN_Tran_Data.IDE = CAN_Id_Standard ;//��׼֡
	CAN_Tran_Data.DLC=0x08;
	CAN_Transmit(CAN1,&CAN_Tran_Data);
}

/*-------------------------------------------------------------------------------------------------
�� �� ����CAN_SendData2RED
�������ܣ�ͨ��can1��������
��    ע���Դ�췢������
-------------------------------------------------------------------------------------------------*/
void CAN_SendData2RED(u32 ID,s32 Current)
{
	CAN_Tran_Data.Data[0]= (u8)((Current>>24)&0xFF);
	CAN_Tran_Data.Data[1]=(u8)((Current>>16)&0xFF);
	CAN_Tran_Data.Data[2]= (u8)((Current>>8)&0xFF);
	CAN_Tran_Data.Data[3]= (u8)(Current&0xFF);
	CAN_Tran_Data.StdId=ID;
	CAN_Tran_Data.RTR = CAN_RTR_Data;//����֡
	CAN_Tran_Data.IDE = CAN_Id_Standard ;//��׼֡
	CAN_Tran_Data.DLC=0x08;
	CAN_Transmit(CAN1,&CAN_Tran_Data);
}

/*-------------------------------------------------------------------------------------------------
�� �� ����GetEncoderNumber
�������ܣ�����6025��6623��RM3510����巵�صĻ�е�Ƕ�ֵ������ʽ������ֵ��
��    ע����е�Ƕ�ֵ��Χ��0~8191��0x1FFF��
-------------------------------------------------------------------------------------------------*/

s32 Get_Encoder_Number(CanRxMsg* rx_message)
{
  s32 encoder_temp;
	encoder_temp = rx_message->Data[0]<<24 | rx_message->Data[1]<<16 | rx_message->Data[2]<<8 | rx_message->Data[3] ;
	return encoder_temp;
}

/*-------------------------------------------------------------------------------------------------
�� �� ����CAN1_Encoder_Process
�������ܣ������������
��    ע��
-------------------------------------------------------------------------------------------------*/

void CAN1_Encoder_Process(ST_ENCODER *p,s32 value)
{
	p->siPreRawValue=p->siRawValue;
	p->siRawValue = value;
	p->siDiff = p->siRawValue - p->siPreRawValue;
	if(p->FirstDeal){
		//��һ�ν���
		p->siSumValue=0;
		p->FirstDeal=0;
	}else{
		//�ǵ�һ�ν���
		if((p->siDiff) > MAX_ANGLE){
			//��ת�������
			p->siSumValue += p->siDiff - p->siNumber;
		}else if((p->siDiff) < (-MAX_ANGLE)){
			//��ת�������
			p->siSumValue += p->siDiff + p->siNumber;
		}else {
			p->siSumValue += p->siDiff;
		}
	}
}














