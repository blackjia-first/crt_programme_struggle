#ifndef __CAN_H__
#define	__CAN_H__

#include "main.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define CAN_AIR_ID    0x30	//���׿���ID
#define OPEN_CYLINDER(channel)    ( g_uiAirValve |= (0x01 << (channel-1)) ) ; SendAirMsgByCan2(&g_uiAirValve)       //���׿���
#define CLOSE_CYLINDER(channel)   ( g_uiAirValve &= ( ~(0x01 << (channel-1)) ) ) ; SendAirMsgByCan2(&g_uiAirValve)  //���׹ر�

typedef struct  
{
	uint32_t raw_value;		//���α�������ԭʼֵ
	uint32_t pre_raw_value;	//��һ�α�������ԭʼֵ
	int32_t diff;			//����������ԭʼֵ�Ĳ�ֵ
	uint8_t reduction_ratio;	//���ٱ�
	int16_t speed;		///32��16λ����������λ��λ��
	fp32 sum_value;		//�������ۼ�ֵ
	//encoder_c(uint8_t reduction_ratio__ = 19) : raw_value(0), pre_raw_value(0), diff(0), 
	//										reduction_ratio(reduction_ratio__), speed(0), sum_value(0) {};
}encoder_t;

extern unsigned int g_uiAirValve;	     //�����й�ȫ�ֱ���

extern void SendAirMsgByCan2(unsigned int* pAir);

extern void can2_send_current(int16_t current);
extern void can2_init(void);
extern void get_encoder(encoder_t* p_encoder, CanRxMsg* p_can_rx_msg);

extern encoder_t encoder;

#endif



