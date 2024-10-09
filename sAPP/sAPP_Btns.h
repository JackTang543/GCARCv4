#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "sGenBtnDrv2.h"
#include "sDBG_UART.hpp"


#define SGBD_KEY_UP_ID   0
#define SGBD_KEY_DN_ID   1
#define SGBD_KEY_ET_ID   2
#define SGBD_KEY_BK_ID   3


void sAPP_Btns_Init();

#ifdef __cplusplus
}
#endif
