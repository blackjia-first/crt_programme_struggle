#ifndef __CAN_H__
#define	__CAN_H__

#include "main.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define CAN_AIR_ID    0x30	//气缸控制ID
#define OPEN_CYLINDER(channel)    ( g_uiAirValve |= (0x01 << (channel-1)) ) ; SendAirMsgByCan2(&g_uiAirValve)       //气缸开启
#define CLOSE_CYLINDER(channel)   ( g_uiAirValve &= ( ~(0x01 << (channel-1)) ) ) ; SendAirMsgByCan2(&g_uiAirValve)  //气缸关闭

extern void can2_init(void);


#endif



