#include "sAPP_Motor.hpp"


sAPP_Motor motor;


sAPP_Motor::sAPP_Motor(){

}

sAPP_Motor::~sAPP_Motor(){

}

int sAPP_Motor::init(){
    sDRV_DRV8870_Init();
    sDRV_GMR_Init();

    return 0;
}


void sAPP_Motor::setLM(float percent){
    sDRV_DRV8870_SetLeftPct(percent);
}

float sAPP_Motor::getLRPM(){
    return sDRV_GMR_GetLeftRPM();
}

void sAPP_Motor::setLBrake(bool is_short){
    sDRV_DRV8870_SetLeftBrake(is_short);
}


void sAPP_Motor::setRM(float percent){
    sDRV_DRV8870_SetRightPct(percent);
}

float sAPP_Motor::getRRPM(){
    return sDRV_GMR_GetRightRPM();
}

void sAPP_Motor::setRBrake(bool is_short){
    sDRV_DRV8870_SetRightBrake(is_short);
}



void sAPP_Motor::update(){
    sDRV_GMR_Handler();
}





