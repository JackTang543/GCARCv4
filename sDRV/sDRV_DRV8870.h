#ifndef __SDRV_DRV8870_H__
#define __SDRV_DRV8870_H__
#ifdef __cplusplus
extern "C"{
#endif



#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "sBSP_TIM.h"



void sDRV_DRV8870_Init();
void sDRV_DRV8870_SetLeftBrake(bool is_short);
void sDRV_DRV8870_SetRightBrake(bool is_short);
void sDRV_DRV8870_SetLeftPct(float percent);
void sDRV_DRV8870_SetRightPct(float percent);



#ifdef __cplusplus
}
#endif
#endif
