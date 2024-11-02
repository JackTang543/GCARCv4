
#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"
#include "sBSP_SPI.h"

int sBSP_DMA_MemSetByte_Init();
int sBSP_DMA_MemSetByte(uint8_t value,uint8_t* pDst,uint16_t len_bytes);

void sBSP_DMA2S1_Init(void);
void sBSP_DMA2S1_32MemToSPI1(uint32_t* pSrc,uint32_t len_bytes);



#ifdef __cplusplus
}
#endif

