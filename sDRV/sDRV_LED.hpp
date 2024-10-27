#pragma once

#include "stm32f4xx_hal.h"


typedef struct{
    GPIO_TypeDef* group;
    uint32_t pin;
    uint16_t id;
}LED_List_t;



class sDRV_LED{
public:
    sDRV_LED();
    ~sDRV_LED();

    int init(GPIO_TypeDef* group = GPIOC,uint32_t pin = GPIO_PIN_13,uint16_t id = 0);

    void set(uint16_t id = 0,bool is_light = 1);
    void set(bool is_light = 1);

    void toggle(uint16_t id = 0);


private:
    LED_List_t list[4];


};


extern sDRV_LED led;

