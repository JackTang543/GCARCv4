#pragma once


#include "stm32f4xx_hal.h"


#include "sDRV_DRV8870.h"
#include "sDRV_GMR.h"

class sAPP_Motor{
public:

    sAPP_Motor();
    ~sAPP_Motor();

    int init();

    void setLM(float percent);
    float getLRPM();
    
    void setRM(float percent);
    float getRRPM();


    void update();

private:



};


extern sAPP_Motor motor;



