#include "can.h"

CanRxMsg  CAN_Rece_Data;
CanTxMsg  CAN_Tran_Data;

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 

//F4中时钟默认频率为：AHB=84M  APB1=42M

/*-------------------------------------------------------------------------------------------------
函 数 名：CAN1_Mode_Init
函数功能：配置CAN1的相关寄存器
备    注：波特率、模式由用户自己配置
-------------------------------------------------------------------------------------------------*/
void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;
    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
		CAN_ITConfig (CAN1,CAN_IT_FMP0,ENABLE);
		CAN_ITConfig (CAN1,CAN_IT_TME,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}   
 
 
///*------------------------------------------------------------
//函数名:CAN_Configuration()
//函数功能: 初始化系统CAN总线，CAN1&CAN2
//输入: 无
//输出：无
//备注:
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
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟	
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
//	CAN_InitStructure.CAN_TTCM = DISABLE;//事件触发
//	CAN_InitStructure.CAN_ABOM = ENABLE;//自动离线管理
//	CAN_InitStructure.CAN_AWUM = ENABLE;//自动唤醒
//	CAN_InitStructure.CAN_NART = DISABLE;//错误重传
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
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//接收中断
//	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//接收中断		
//}

/*-------------------------------------------------------------------------------------------------
函 数 名：CAN_SendData
函数功能：通过can1发送数据
备    注：
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
	CAN_Tran_Data.RTR = CAN_RTR_Data;//数据帧
	CAN_Tran_Data.IDE = CAN_Id_Standard ;//标准帧
	CAN_Tran_Data.DLC=0x08;
	CAN_Transmit(CAN1,&CAN_Tran_Data);
}

/*-------------------------------------------------------------------------------------------------
函 数 名：CAN_SendData2RED
函数功能：通过can1发送数据
备    注：对大红发送数据
-------------------------------------------------------------------------------------------------*/
void CAN_SendData2RED(u32 ID,s32 Current)
{
	CAN_Tran_Data.Data[0]= (u8)((Current>>24)&0xFF);
	CAN_Tran_Data.Data[1]=(u8)((Current>>16)&0xFF);
	CAN_Tran_Data.Data[2]= (u8)((Current>>8)&0xFF);
	CAN_Tran_Data.Data[3]= (u8)(Current&0xFF);
	CAN_Tran_Data.StdId=ID;
	CAN_Tran_Data.RTR = CAN_RTR_Data;//数据帧
	CAN_Tran_Data.IDE = CAN_Id_Standard ;//标准帧
	CAN_Tran_Data.DLC=0x08;
	CAN_Transmit(CAN1,&CAN_Tran_Data);
}

/*-------------------------------------------------------------------------------------------------
函 数 名：GetEncoderNumber
函数功能：接收6025、6623或RM3510电调板返回的机械角度值（绝对式编码器值）
备    注：机械角度值范围：0~8191（0x1FFF）
-------------------------------------------------------------------------------------------------*/

s32 Get_Encoder_Number(CanRxMsg* rx_message)
{
  s32 encoder_temp;
	encoder_temp = rx_message->Data[0]<<24 | rx_message->Data[1]<<16 | rx_message->Data[2]<<8 | rx_message->Data[3] ;
	return encoder_temp;
}

/*-------------------------------------------------------------------------------------------------
函 数 名：CAN1_Encoder_Process
函数功能：码盘溢出处理
备    注：
-------------------------------------------------------------------------------------------------*/

void CAN1_Encoder_Process(ST_ENCODER *p,s32 value)
{
	p->siPreRawValue=p->siRawValue;
	p->siRawValue = value;
	p->siDiff = p->siRawValue - p->siPreRawValue;
	if(p->FirstDeal){
		//第一次进入
		p->siSumValue=0;
		p->FirstDeal=0;
	}else{
		//非第一次进入
		if((p->siDiff) > MAX_ANGLE){
			//反转向下溢出
			p->siSumValue += p->siDiff - p->siNumber;
		}else if((p->siDiff) < (-MAX_ANGLE)){
			//正转向上溢出
			p->siSumValue += p->siDiff + p->siNumber;
		}else {
			p->siSumValue += p->siDiff;
		}
	}
}














