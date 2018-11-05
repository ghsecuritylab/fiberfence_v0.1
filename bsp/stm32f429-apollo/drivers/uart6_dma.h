#ifndef _UART6_DMA_H
#define _UART6_DMA_H

#include "stm32f4xx.h"

extern DMA_HandleTypeDef  UART6TxDMA_Handler;      //DMA¾ä±ú
extern UART_HandleTypeDef UART6_Handler; //UART¾ä±ú

void uart6_dma_init(uint32_t bound);

#endif
