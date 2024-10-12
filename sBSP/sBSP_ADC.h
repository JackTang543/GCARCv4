#pragma once
#ifdef __cplusplus
extern "C" {
#endif



#include "stm32f4xx_hal.h"




void sBSP_ADC_Init();
float sBSP_ADC_GetBatVolt();


#ifdef __cplusplus
}
#endif



