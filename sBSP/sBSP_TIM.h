#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"
#include <stdbool.h>


extern TIM_HandleTypeDef g_htim2;


void sBSP_TIM_Motor_Init();
void sBSP_TIM_Motor_SetPWMFreq(uint32_t freq);
void sBSP_TIM_Motor_L1SetEN(bool is_en);
void sBSP_TIM_Motor_L2SetEN(bool is_en);
void sBSP_TIM_Motor_R1SetEN(bool is_en);
void sBSP_TIM_Motor_R2SetEN(bool is_en);
void sBSP_TIM_Motor_L1SetDuty(float percent);
void sBSP_TIM_Motor_L2SetDuty(float percent);
void sBSP_TIM_Motor_R1SetDuty(float percent);
void sBSP_TIM_Motor_R2SetDuty(float percent);



#ifdef __cplusplus
}
#endif
