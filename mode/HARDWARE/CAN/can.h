#ifndef __CAN_H__
#define	__CAN_H__

#include "main.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define CAN_AIR_ID    0x30	//气缸控制ID
#define OPEN_CYLINDER(channel)    ( g_uiAirValve |= (0x01 << (channel-1)) ) ; SendAirMsgByCan2(&g_uiAirValve)       //气缸开启
#define CLOSE_CYLINDER(channel)   ( g_uiAirValve &= ( ~(0x01 << (channel-1)) ) ) ; SendAirMsgByCan2(&g_uiAirValve)  //气缸关闭

typedef struct  
{
	uint32_t raw_value;		//本次编码器的原始值
	uint32_t pre_raw_value;	//上一次编码器的原始值
	int32_t diff;			//编码器两次原始值的差值
	uint8_t reduction_ratio;	//减速比
	int16_t speed;		///32和16位变量，符号位的位置
	fp32 sum_value;		//编码器累加值
	//encoder_c(uint8_t reduction_ratio__ = 19) : raw_value(0), pre_raw_value(0), diff(0), 
	//										reduction_ratio(reduction_ratio__), speed(0), sum_value(0) {};
}encoder_t;

extern unsigned int g_uiAirValve;	     //气缸有关全局变量

extern void SendAirMsgByCan2(unsigned int* pAir);

extern void can2_send_current(int16_t current);
extern void can2_init(void);
extern void get_encoder(encoder_t* p_encoder, CanRxMsg* p_can_rx_msg);

extern encoder_t encoder;

#endif



