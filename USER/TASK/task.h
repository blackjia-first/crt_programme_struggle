#ifndef __TASK_H__
#define __TASK_H__

#include "pid.h"

typedef struct
{
	PidTypeDef motor_speed_pid[4];
	PidTypeDef motor_angle_pid[4];
}motor_pid_t;

extern motor_pid_t motor_pid;

#endif

