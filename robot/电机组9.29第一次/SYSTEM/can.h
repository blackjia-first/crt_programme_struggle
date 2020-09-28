#ifndef __CAN_H__
#define	__CAN_H__


#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

typedef struct
{
	u32 RawValue;		//本次编码器的原始值
	u32 PreRawValue;	//上一次编码器的原始值
	s32 Diff;			//编码器两次原始值的差值
	s32 Number;			//编码器线数
	float SumValue;		//编码器累加值
}ST_ENCODER;

#define CAN_AIR_ID 0x30	//气缸控制ID
#define CAN_MOTOR_ID 0x200
#define OPEN_CYLINDER(channel)    ( g_uiAirValve |= (0x01 << (channel-1)) ) ; SendAirMsgByCan2(&g_uiAirValve)       //气缸开启
#define CLOSE_CYLINDER(channel)   ( g_uiAirValve &= ( ~(0x01 << (channel-1)) ) ) ; SendAirMsgByCan2(&g_uiAirValve)  //气缸关闭

extern unsigned int g_uiAirValve;	     //气缸有关全局变量

void Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value);

void SendAirMsgByCan2(unsigned int* pAir);
void CAN2_Configuration(void);

s16 GetSpeed(CanRxMsg* pCanRxMsg);
u16 GetEncoderNumber(CanRxMsg* pCanRxMsg);

extern void CAN_SendCurrent(s16 current);

#endif



