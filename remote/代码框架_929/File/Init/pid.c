#include "main.h"
#include "pid.h"

#define LimitMax(input, max)   \
	{                          \
		if (input > max)       \
		{                      \
			input = max;       \
		}                      \
		else if (input < -max) \
		{                      \
			input = -max;      \
		}                      \
	}	
	
void pid_init()
{

}

void CalISeparatedPID()
{   

}
