#ifndef __CAN_H__
#define	__CAN_H__

#include "main.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#define CAN_AIR_ID    0x30	//���׿���ID
#define OPEN_CYLINDER(channel)    ( g_uiAirValve |= (0x01 << (channel-1)) ) ; SendAirMsgByCan2(&g_uiAirValve)       //���׿���
#define CLOSE_CYLINDER(channel)   ( g_uiAirValve &= ( ~(0x01 << (channel-1)) ) ) ; SendAirMsgByCan2(&g_uiAirValve)  //���׹ر�

extern void can2_init(void);


#endif



