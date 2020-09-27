#ifndef __KEY_H__
#define __KEY_H__


#include "stm32f4xx.h"

enum
{
	  ON = 0,
	  OFF
};

typedef struct 
{
	  uint8_t key0, key1;
	  uint8_t enter_key0_from_key1;
}key_status_t;

extern void extix_init(void);
extern void key_init(void);

extern key_status_t key_status;

#endif
