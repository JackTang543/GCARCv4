#include "sGCARCv4_hal_msp.h"




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
