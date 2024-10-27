#include "sDRV_LED.hpp"
#include "sGCARCv4_Def.h"



sDRV_LED led;


sDRV_LED::sDRV_LED(){

}

sDRV_LED::~sDRV_LED(){

}

int sDRV_LED::init(GPIO_TypeDef* group,uint32_t pin,uint16_t id){
    if(group == nullptr){
        return -2;
    }
    
    if(group == GPIOA){
        __GPIOA_CLK_ENABLE();
    }
    else if (group == GPIOB){
        __GPIOB_CLK_ENABLE();
    }
    else if (group == GPIOC){
        __GPIOC_CLK_ENABLE();
    }
    else if (group == GPIOD){
        __GPIOD_CLK_ENABLE();
    }
    else{
        return -3;
    }

    list[id].group = group;
    list[id].pin   = pin;
    list[id].id = id;
    
    
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = USER_LED_Pin;
    HAL_GPIO_Init(USER_LED_GPIO_Port,&gpio);

    return 0;
}

void sDRV_LED::set(uint16_t id,bool is_light){
    HAL_GPIO_WritePin(list[id].group,list[id].pin,(GPIO_PinState)is_light);
}

void sDRV_LED::set(bool is_light){
    HAL_GPIO_WritePin(list[0].group,list[0].pin,(GPIO_PinState)is_light);
}

void sDRV_LED::toggle(uint16_t id){
    HAL_GPIO_TogglePin(list[id].group,list[id].pin);
}





