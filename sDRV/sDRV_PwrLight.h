#pragma once
#ifdef __cplusplus
extern "C"{
#endif



#include "stm32f4xx_hal.h"

#include "sBSP_TIM.h"

void sDRV_PL_Init();
void sDRV_PL_SetBrightness(float percent);



#ifdef __cplusplus
}
#endif

