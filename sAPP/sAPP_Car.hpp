#pragma once


#include "stm32f4xx_hal.h"



class sAPP_Car{
public:
    sAPP_Car();
    ~sAPP_Car();


    uint32_t coreClk;


    int initSys();
    int initBoard();
    


private:


};


extern sAPP_Car car;



