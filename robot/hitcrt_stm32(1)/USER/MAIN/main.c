#include "main.h"


int main(void)
{
	ALL_Iinitialization();
	while(1)
	{
		
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
