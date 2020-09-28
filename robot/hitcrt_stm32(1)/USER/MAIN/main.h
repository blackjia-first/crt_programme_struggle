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
	uint32_t raw_value;		//���α�������ԭʼֵ
	uint32_t pre_raw_value;	//��һ�α�������ԭʼֵ
	int32_t diff;			//����������ԭʼֵ�Ĳ�ֵ
	uint8_t reduction_ratio;	//���ٱ�
	int16_t speed;		///32��16λ����������λ��λ��
	float sum_value;		//�������ۼ�ֵ

}encoder_t;

extern unsigned int g_uiAirValve;	     //�����й�ȫ�ֱ���

extern void SendAirMsgByCan2(unsigned int* pAir);

extern void can2_send_current(int16_t current);

extern void get_encoder(encoder_t* p_encoder, CanRxMsg* p_can_rx_msg);

extern encoder_t encoder;



#endif

