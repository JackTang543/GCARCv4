#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"
#include <stdbool.h>


extern TIM_HandleTypeDef g_htim2;
extern TIM_HandleTypeDef g_htim3;


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


void sBSP_TIM_GMRR_Init();
uint32_t sBSP_TIM_GMRR_Get();
void sBSP_TIM_GMRR_Set(uint32_t count);
void sBSP_TIM_GMRR_SetEN(bool is_en);

void sBSP_TIM_GMRL_Init();
uint32_t sBSP_TIM_GMRL_Get();
void sBSP_TIM_GMRL_Set(uint32_t count);
void sBSP_TIM_GMRL_SetEN(bool is_en);


void sBSP_TIM_Light_Init();
void sBSP_TIM_Light_SetPWMFreq(uint32_t freq);
void sBSP_TIM_Light_SetEN(bool is_en);
void sBSP_TIM_Light_Set(float percent);



#ifdef __cplusplus
}
#endif
