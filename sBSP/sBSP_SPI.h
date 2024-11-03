#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include "stm32f4xx_it.h"
    
extern SPI_HandleTypeDef hspi1;


/*OLED SPI interface*/
int sBSP_SPI_OLED_Init(uint32_t SPI_BAUDRATE);
void sBSP_SPI_OLED_SetCS(bool cs_lv);
void sBSP_SPI_OLED_SetEN(uint8_t en);
void sBSP_SPI_OLED_SendByte(uint8_t byte);
uint8_t sBSP_SPI_OLED_RecvByte();
void sBSP_SPI_OLED_SendBytes(uint8_t *pData,uint16_t Size);
bool sBSP_SPI_OLED_IsIdle();

void sBSP_SPI_OLED_RecvBytes(uint8_t *pData,uint16_t Size);


/*IMU SPI interface*/
void sBSP_SPI_IMU_Init(uint32_t SPI_BAUDRATE);
void sBSP_SPI_IMU_SetEN(uint8_t en);
void sBSP_SPI_IMU_SendByte(uint8_t byte);
uint8_t sBSP_SPI_IMU_RecvByte();
void sBSP_SPI_IMU_SendBytes(uint8_t *pData,uint16_t Size);
void sBSP_SPI_IMU_RecvBytes(uint8_t *pData,uint16_t Size);


/*TRACK SPI interface*/
void sBSP_SPI_TRACK_Init(uint32_t SPI_BAUDRATE);
void sBSP_SPI_TRACK_SetEN(uint8_t en);
void sBSP_SPI_TRACK_SendByte(uint8_t byte);
uint8_t sBSP_SPI_TRACK_RecvByte();
void sBSP_SPI_TRACK_SendBytes(uint8_t *pData,uint16_t Size);
void sBSP_SPI_TRACK_RecvBytes(uint8_t *pData,uint16_t Size);


void sBSP_SPI_PS2_Init(uint32_t SPI_BAUDRATE);
void sBSP_SPI_PS2_SetEN(uint8_t en);
uint8_t sBSP_SPI_PS2_TransferByte(uint8_t send);



#ifdef __cplusplus
}
#endif

