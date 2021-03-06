#include "sys.h"
#include "usart.h"
#include "string.h"
#include "led.h"

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

//FILE __stdout;       
////定义_sys_exit()以避免使用半主机模式    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
//重定义fputc函数 

#if defined(__cplusplus)
namespace std
{
#endif

int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

#if defined(__cplusplus)
}
#endif


uart1_t uart1;

void uart_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	memset(uart1.buf, 0, sizeof(uart1.buf));
	memset(uart1.data_to_handle, 0, sizeof(uart1.data_to_handle));
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
    USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}

static void uart_send_byte(USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void usart_send_string(USART_TypeDef * pUSARTx, char *str)
{
	uint32_t k=0;
	do 
	{
		uart_send_byte( pUSARTx, *(str + k) );
		k++;
	}while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void data_handle(uint8_t data)
{
	static uint8_t status = FREE1, i = 0, len = 0;
	
	switch(status)
	{
		case FREE1:
			if(data == 0x55) status = FREE2;
			else status = FREE1;
			break;
		
		case FREE2:
			if(data == 0x11) status = LENGTH;
			else status = FREE1;
			break;
		
		case LENGTH:
			len = data;
			status = DATA;
			break;
		
		case DATA:
			uart1.data_to_handle[i] = data;
			i++;
			if(i == len)
			{
				status = TAIL;
			}
			break;
			
		case TAIL:
			if(data == 0xAA)
			{
				led1_on();
				led2_on();
				//usart_send_string(USART1,"接收到正确数据");
				//数据处理
			}
			else
			{
				memset(uart1.data_to_handle, 0, sizeof(uart1.data_to_handle));
			}
			i = 0;
			len = 0;
			status = FREE1;
			break;
	}
}
