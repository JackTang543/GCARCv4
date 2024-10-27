#include "sDRV_PS2.h"


//
#define SPI_CS_CLK_EN    __GPIOD_CLK_ENABLE
#define SPI_CS_PORT      TRACK_CS_GPIO_Port
#define SPI_CS_PIN       TRACK_CS_Pin




static void portSPIInit(){
    sBSP_SPI_TRACK_Init(SPI_BAUDRATEPRESCALER_256);
    sBSP_SPI_TRACK_SetEN(1);
}

static void portCSInit(){
    SPI_CS_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = SPI_CS_PIN;
    HAL_GPIO_Init(SPI_CS_PORT,&gpio);
}

static inline void portSetCS(bool lv){
    HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,(GPIO_PinState)lv);
}


static inline uint8_t portReadByte(){
    return sBSP_SPI_TRACK_RecvByte();
}

static inline void portSendByte(uint8_t byte){
    sBSP_SPI_TRACK_SendByte(byte);
}





#include "sDBG_Debug.h"




int sDRV_PS2_Init(){
    portSPIInit();
    portCSInit();
    portSetCS(1);
    HAL_Delay(1);


    portSetCS(0);
    HAL_Delay(1);
    portSendByte(0x01);
    HAL_Delay(1);
    uint8_t tmp = portReadByte();
    HAL_Delay(1);
    portSetCS(1);

    sDBG_Debug_Printf("tmp=0x%2X\n", tmp);



    return 0;
}





void sDRV_PS2_Handler(){

}





