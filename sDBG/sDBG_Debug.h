#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"


void sDBG_Debug_Printf(const char *fmt,...);

void Error_Handler();


#ifdef __cplusplus
}
#endif

