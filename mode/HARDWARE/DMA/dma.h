#ifndef __DMA_H__
#define __DMA_H__

extern uint8_t rxbuf[8];

extern void set_dma_rx_ndtr(void);
extern void set_dma_tx_ndtr(void);
extern void dma_init(void);

#endif
