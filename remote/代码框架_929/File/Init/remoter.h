#ifndef __REMOTER_H__
#define __REMOTER_H__

typedef enum
{
	UP,
	DOWN,
	STILL1
}motor1_e;

typedef enum
{
	LEFT,
	RIGHT,
	STILL2
}motor2_e;

typedef enum
{
	OPEN,
	CLOSE
}cylinder_e;

extern motor1_e motor1;
extern motor2_e motor2;
extern cylinder_e cylinder12;
extern cylinder_e cylinder3;

extern void remoter_init(void);

#endif
