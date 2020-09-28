#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include <stdbool.h>
#include "all_init.h"
#include "math.h"
#define NULL 0 

#define FALSE false
#define TRUE true

typedef float fp32;
typedef double fp64;


typedef struct  
{
	uint32_t raw_value;		//本次编码器的原始值
	uint32_t pre_raw_value;	//上一次编码器的原始值
	int32_t diff;			//编码器两次原始值的差值
	uint8_t reduction_ratio;	//减速比
	int16_t speed;		///32和16位变量，符号位的位置
	float sum_value;		//编码器累加值

}encoder_t;

extern unsigned int g_uiAirValve;	     //气缸有关全局变量

extern void SendAirMsgByCan2(unsigned int* pAir);

extern void can2_send_current(int16_t current);

extern void get_encoder(encoder_t* p_encoder, CanRxMsg* p_can_rx_msg);

extern encoder_t encoder;



#endif

