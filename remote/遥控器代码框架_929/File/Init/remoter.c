#ifdef REMOTER

#include "remoter.h"
#include "usart.h"
#include "dma.h"

motor1_e motor1;
motor2_e motor2;
cylinder_e cylinder12;
cylinder_e cylinder3;

void remoter_init(void)
{
	motor1 = STILL1;
	motor2 = STILL2;
	cylinder12 = CLOSE;
	cylinder3 = CLOSE;
	usart3_init(9600);
	dma_usart3_init();
	uart_send_byte(USART3, 0x45);//如果遥控器接收到该数据，证明已经成功连接
}


#endif
