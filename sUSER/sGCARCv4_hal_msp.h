#pragma once



#include "stm32f4xx_hal.h"



void HAL_UART_MspInit(UART_HandleTypeDef* huart);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);


