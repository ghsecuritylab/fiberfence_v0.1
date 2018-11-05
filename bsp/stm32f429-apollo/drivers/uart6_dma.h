#ifndef _UART6_DMA_H
#define _UART6_DMA_H

#include "stm32f4xx.h"

extern DMA_HandleTypeDef  UART6TxDMA_Handler;      //DMA���
extern UART_HandleTypeDef UART6_Handler; //UART���

void uart6_dma_init(uint32_t bound);

#endif
