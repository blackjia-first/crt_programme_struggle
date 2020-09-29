#ifndef __DMA_H__
#define __DMA_H__

extern uint8_t rxbuf[3];

extern void set_dma_usart3_rx_ndtr(void);
extern void dma_usart3_init(void);

#endif
