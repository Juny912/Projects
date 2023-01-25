#ifndef __MYDMA_H
#define __MYDMA_H

#include "stm32f10x_dma.h"

void MyDMA_Configure(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);

void MyDMA_Enable(DMA_Channel_TypeDef *DMA_CHx);


#endif
