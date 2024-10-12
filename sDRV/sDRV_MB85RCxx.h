#pragma once
#ifdef __cplusplus
extern "C"{
#endif

//sDRV_MB85RCxx.c
//Sightseer's MB85RCxx铁电驱动

#include "stm32f4xx_hal.h"



int8_t sDRV_MB85RCxx_Init();
int8_t sDRV_MB85RCxx_WriteByte(uint16_t addr,uint8_t byte);
int8_t sDRV_MB85RCxx_WriteBytes(uint16_t addr,uint8_t* pData,uint16_t length);
uint8_t sDRV_MB85RCxx_ReadByte(uint16_t addr);
int8_t sDRV_MB85RCxx_ReadBytes(uint16_t addr,uint8_t* pData,uint16_t length);
int8_t sDRV_MB85RCxx_Format(uint8_t all_val);

#ifdef __cplusplus
}
#endif
