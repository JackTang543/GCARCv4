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
    else if(tim_pwmHandle->Instance==TIM12){
        __HAL_RCC_TIM12_CLK_ENABLE();
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
    else if(timHandle->Instance==TIM12){
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM12 GPIO Configuration
    PB14     ------> TIM12_CH1
    */
    GPIO_InitStruct.Pin = LIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(LIGHT_GPIO_Port, &GPIO_InitStruct);
    }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(tim_encoderHandle->Instance==TIM3){
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM3 GPIO Configuration
        PB4     ------> TIM3_CH1
        PB5     ------> TIM3_CH2
        */
        GPIO_InitStruct.Pin = GMR_RA_Pin|GMR_RB_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else if(tim_encoderHandle->Instance==TIM4){
        __HAL_RCC_TIM4_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM4 GPIO Configuration
        PB6     ------> TIM4_CH1
        PB7     ------> TIM4_CH2
        */
        GPIO_InitStruct.Pin = GMR_LA_Pin|GMR_LB_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}



void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hi2c->Instance == I2C1){
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB8     ------> I2C1_SCL
        PB9     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = BI2C_SCL_Pin|BI2C_SDA_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        __HAL_RCC_I2C1_CLK_ENABLE();

        // HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 0);
        // HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        // HAL_NVIC_SetPriority(I2C1_ER_IRQn, 1, 0);
        // HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    }
    else if(hi2c->Instance == I2C2){
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin = EI2C_SCL_Pin|EI2C_SDA_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        __HAL_RCC_I2C2_CLK_ENABLE();

        // HAL_NVIC_SetPriority(I2C2_EV_IRQn, 1, 0);
        // HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        // HAL_NVIC_SetPriority(I2C2_ER_IRQn, 1, 0);
        // HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
    }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(spiHandle->Instance==SPI1){
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA7     ------> SPI1_MOSI
        PA15     ------> SPI1_NSS
        */
        GPIO_InitStruct.Pin = OLED_SCK_Pin|OLED_SDA_Pin|OLED_CS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        }
    else if(spiHandle->Instance==SPI2){
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI2 GPIO Configuration
        PC2     ------> SPI2_MISO
        PC3     ------> SPI2_MOSI
        PB13     ------> SPI2_SCK
        */
        GPIO_InitStruct.Pin = IMU_MISO_Pin|IMU_MOSI_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = IMU_SCK_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(IMU_SCK_GPIO_Port, &GPIO_InitStruct);
    }
    else if(spiHandle->Instance==SPI3){
        __HAL_RCC_SPI3_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */
        GPIO_InitStruct.Pin = TRACK_SCK_Pin|TRACK_MISO_Pin|TRACK_MOSI_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }
}


void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(adcHandle->Instance==ADC1){
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA4     ------> ADC1_IN4
    */
    GPIO_InitStruct.Pin = VBAT_ADC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VBAT_ADC_GPIO_Port, &GPIO_InitStruct);
    }
}






