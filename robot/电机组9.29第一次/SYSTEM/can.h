#ifndef __CAN_H__
#define	__CAN_H__


#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

typedef struct
{
	u32 RawValue;		//���α�������ԭʼֵ
	u32 PreRawValue;	//��һ�α�������ԭʼֵ
	s32 Diff;			//����������ԭʼֵ�Ĳ�ֵ
	s32 Number;			//����������
	float SumValue;		//�������ۼ�ֵ
}ST_ENCODER;

#define CAN_AIR_ID 0x30	//���׿���ID
#define CAN_MOTOR_ID 0x200
#define OPEN_CYLINDER(channel)    ( g_uiAirValve |= (0x01 << (channel-1)) ) ; SendAirMsgByCan2(&g_uiAirValve)       //���׿���
#define CLOSE_CYLINDER(channel)   ( g_uiAirValve &= ( ~(0x01 << (channel-1)) ) ) ; SendAirMsgByCan2(&g_uiAirValve)  //���׹ر�

extern unsigned int g_uiAirValve;	     //�����й�ȫ�ֱ���

void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value);

void SendAirMsgByCan2(unsigned int* pAir);
void CAN2_Configuration(void);

s16 GetSpeed(CanRxMsg* pCanRxMsg);
u16 GetEncoderNumber(CanRxMsg* pCanRxMsg);

extern void CAN_SendCurrent(s16 current);

#endif



