#pragma once



#include "stm32f4xx_hal.h"



void HAL_UART_MspInit(UART_HandleTypeDef* huart);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle);



