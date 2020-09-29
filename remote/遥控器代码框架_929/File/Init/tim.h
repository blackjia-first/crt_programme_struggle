#ifndef __TIM_H__
#define __TIM_H__

#include "main.h"

//tim2_init(5000-1,8400-1);
//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms     

void tim2_init(uint16_t arr,uint16_t psc);

#endif
