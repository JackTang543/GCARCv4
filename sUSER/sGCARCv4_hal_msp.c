#include "sGCARCv4_hal_msp.h"

#include "sGCARCv4_PinDefine.h"


//这些是串口的GPIO口
#define UART1_GPIO       GPIOA
#define UART1_TX_PIN     GPIO_PIN_9
#define UART1_RX_PIN     GPIO_PIN_10

//override the weak definitions provided by the HAL lib
void HAL_UART_MspInit(UART_HandleTypeDef* huart){
    if(huart->Instance == USART1){
        __GPIOA_CLK_ENABLE();
        __USART1_CLK_ENABLE();

        GPIO_InitTypeDef gpio;
        gpio.Mode      = GPIO_MODE_AF_PP;
        gpio.Pin       = UART1_TX_PIN | UART1_RX_PIN;
        gpio.Pull      = GPIO_NOPULL;
        gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(UART1_GPIO,&gpio);
        
        // HAL_NVIC_SetPriority(USART1_IRQn,0,0);
        // HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle){
    if(tim_pwmHandle->Instance==TIM2){
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(timHandle->Instance==TIM2){
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA0-WKUP     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        PA2     ------> TIM2_CH3
        PA3     ------> TIM2_CH4
        */
        GPIO_InitStruct.Pin = ML_PWM1_Pin|ML_PWM2_Pin|MR_PWM1_Pin|MR_PWM2_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}



