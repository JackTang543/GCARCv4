#ifndef __SDRV_GMR_H__
#define __SDRV_GMR_H__
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"


void sDRV_GMR_Init();

float sDRV_GMR_GetLeftRPM();
float sDRV_GMR_GetRightRPM();
void sDRV_GMR_Handler();

#ifdef __cplusplus
}
#endif
#endif