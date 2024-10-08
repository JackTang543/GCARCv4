#include "main.h"



//128+64K SRAM
//1024K FLASH
//168MHz 210DMIPS

//compile env:
//AC6 C++14


int main(){
    HAL_Init();
    HAL_InitTick(0);
    
    float32_t a = -1.5f;
    float32_t b = 0;

    arm_abs_f32(&a,&b,1);


    
    b = a + 50.0f;


    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC,&gpio);

    
    sDBG_UART dbgUart;

    UART_HandleTypeDef g_husart1;

    dbgUart.init(USART1);

    dbgUart.print(520);
    dbgUart.println(1314);



    while(1){
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        HAL_Delay(100);

    }
}





