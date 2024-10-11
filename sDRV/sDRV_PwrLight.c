#include "sDRV_PwrLight.h"

#include "sLib.h"


void sDRV_PL_Init(){
    sBSP_TIM_Light_Init();
    sBSP_TIM_Light_SetPWMFreq(5000);
    sBSP_TIM_Light_SetEN(1);
}


void sDRV_PL_SetBrightness(float percent){
    sBSP_TIM_Light_Set(sLib_GammaCorrect(percent));
}






