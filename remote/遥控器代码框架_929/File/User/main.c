/*******************************************************************************************************************
@@暑期竞培营结营项目比赛程序
	@<1>
	@<2>
	@<3>

*******************************************************************************************************************/

#ifndef REMOTER

#include "main.h"

encoder_t encoder = {.sum_value = 0, .raw_value = 0, .speed = 0, .reduction_ratio = 19};//注意看拿的电机减速比是多少
unsigned int g_uiAirValve;	     //气缸有关全局变量

int main(void)
{
	ALL_Iinitialization();
	while(1)
	{
		//暂定气缸控制格式
		OPEN_CYLINDER(1);
		delay_ms(500);
		CLOSE_CYLINDER(1);
		delay_ms(500);
		OPEN_CYLINDER(2);
		delay_ms(500);
		CLOSE_CYLINDER(2);
		delay_ms(500);
		OPEN_CYLINDER(3);
		delay_ms(500);
		CLOSE_CYLINDER(3);
		delay_ms(500);
//		LED_1_OFF();
//     	LED_2_OFF();
	}
}


/*--------------------------------------------------------------------------------------------------------------------
函数名:SendAirMsgByCan2()
函数功能: 通过CAN2发送气缸的变量
输入: pAir 要控制的气缸ID
输出：无
--------------------------------------------------------------------------------------------------------------------*/
void SendAirMsgByCan2(unsigned int* pAir)
{
	static CanTxMsg TxMessage;
	static unsigned int s_ucLastAir = 0;
	TxMessage.StdId = CAN_AIR_ID;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = 0x04;//发送四个字节的数据
	
		
	s_ucLastAir = *pAir;
//	*((unsigned int*)TxMessage.Data) = s_ucLastAir;
	
	TxMessage.Data[0] = (uint8_t)(s_ucLastAir);
	TxMessage.Data[1] = (uint8_t)(s_ucLastAir>>8);
	TxMessage.Data[2] = (uint8_t)(s_ucLastAir>>16);
	TxMessage.Data[3] = (uint8_t)(s_ucLastAir>>24);
	TxMessage.Data[4] = 0;
	TxMessage.Data[5] = 0;
	TxMessage.Data[6] = 0;
	TxMessage.Data[7] = 0;
	CAN_Transmit(CAN2,&TxMessage);
}


/*************************************************************
函 数 名：get_encoder
函数功能：得到码盘值
备    注：
**************************************************************/
void get_encoder(encoder_t* p_encoder, CanRxMsg* p_can_rx_msg)
{
	p_encoder->raw_value = (p_can_rx_msg->Data[0] << 8) | p_can_rx_msg->Data[1];
	p_encoder->speed = (p_can_rx_msg->Data[2] << 8) | p_can_rx_msg->Data[3];
	p_encoder->diff = p_encoder->raw_value - p_encoder->pre_raw_value;
	p_encoder->sum_value += p_encoder->diff;
	if(p_encoder->diff > 4000)
	{
		p_encoder->sum_value -= 8192; 
	}
	else if(p_encoder->diff < -4000)
	{
		p_encoder->sum_value += 8192;
	}
	
	p_encoder->pre_raw_value = p_encoder->raw_value;
}

/*************************************************************
函 数 名：can2_send_current
函数功能：发送电流函数
备    注：
**************************************************************/
void can2_send_current(int16_t current)
{
	static CanTxMsg motor_msg;
	motor_msg.StdId = 0x200;
	motor_msg.IDE = CAN_Id_Standard;
	motor_msg.RTR = CAN_RTR_Data;
	motor_msg.DLC = 0x08;
	
	motor_msg.Data[0] = (int8_t)(current>>8);
	motor_msg.Data[1] = (int8_t)(current);
	motor_msg.Data[2] = 0;
	motor_msg.Data[3] = 0;
	motor_msg.Data[4] = 0;
	motor_msg.Data[5] = 0;
	motor_msg.Data[6] = 0;
	motor_msg.Data[7] = 0;
	
	CAN_Transmit(CAN2,&motor_msg);

}

#else

#include "main.h"
#include "remoter.h"

encoder_t encoder = {.sum_value = 0, .raw_value = 0, .speed = 0, .reduction_ratio = 19};//注意看拿的电机减速比是多少

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级(0-3共4级)，2位子优先级(0-3共4级)
	delay_init(168);
	can2_init();
	remoter_init();
	//pid_init
	while(1)
	{
		switch(motor1)
		{
		case STILL1:
			break;
		case UP:
			break;
		case DOWN:
			break;
		}
		switch(motor2)
		{
		case STILL2:
			break;
		case LEFT:
			break;
		case RIGHT:
			break;
		}
		switch(cylinder12)
		{
		case OPEN:
			break;
		case CLOSE:
			break;
		}
		switch(cylinder3)
		{
		case OPEN:
			break;
		case CLOSE:
			break;
		}
	}
}


void get_encoder(encoder_t* p_encoder, CanRxMsg* p_can_rx_msg)
{
	p_encoder->raw_value = (p_can_rx_msg->Data[0] << 8) | p_can_rx_msg->Data[1];
	p_encoder->speed = (p_can_rx_msg->Data[2] << 8) | p_can_rx_msg->Data[3];
	p_encoder->diff = p_encoder->raw_value - p_encoder->pre_raw_value;
	p_encoder->sum_value += p_encoder->diff;
	if(p_encoder->diff > 4000)
	{
		p_encoder->sum_value -= 8192; 
	}
	else if(p_encoder->diff < -4000)
	{
		p_encoder->sum_value += 8192;
	}
	
	p_encoder->pre_raw_value = p_encoder->raw_value;
}

/*************************************************************
函 数 名：can2_send_current
函数功能：发送电流函数
备    注：
**************************************************************/
void can2_send_current(int16_t current)
{
	static CanTxMsg motor_msg;
	motor_msg.StdId = 0x200;
	motor_msg.IDE = CAN_Id_Standard;
	motor_msg.RTR = CAN_RTR_Data;
	motor_msg.DLC = 0x08;
	
	motor_msg.Data[0] = (int8_t)(current>>8);
	motor_msg.Data[1] = (int8_t)(current);
	motor_msg.Data[2] = 0;
	motor_msg.Data[3] = 0;
	motor_msg.Data[4] = 0;
	motor_msg.Data[5] = 0;
	motor_msg.Data[6] = 0;
	motor_msg.Data[7] = 0;
	
	CAN_Transmit(CAN2,&motor_msg);

}

#endif

