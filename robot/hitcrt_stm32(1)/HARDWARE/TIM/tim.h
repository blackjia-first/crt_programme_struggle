#ifndef __TIM_H__
#define __TIM_H__

#include "main.h"

//tim2_init(5000-1,8400-1);
//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms     

void tim2_init(uint16_t arr,uint16_t psc);

#endif
