#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

#include "stm32f4xx_it.h"
    
extern SPI_HandleTypeDef hspi1;


/*IMU SPI interface*/
void sBSP_SPI_IMU_Init(uint32_t SPI_BAUDRATE);
void sBSP_SPI_IMU_SendByte(uint8_t byte);
uint8_t sBSP_SPI_IMU_RecvByte();
void sBSP_SPI_IMU_SendBytes(uint8_t *pData,uint16_t Size);
void sBSP_SPI_IMU_RecvBytes(uint8_t *pData,uint16_t Size);


#ifdef __cplusplus
}
#endif

