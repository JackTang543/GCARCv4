#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include "sBSP_SPI.h"
#include "sGCARCv4_Def.h"
#include <stdbool.h>




int sDRV_PS2_Init();
void sDRV_PS2_Handler();




#ifdef __cplusplus
}
#endif
