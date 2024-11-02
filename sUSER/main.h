#pragma once


#include "stm32f4xx_hal.h"

#include "sGCARCv4_hal_msp.h"
#include "sGCARCv4_Def.h"

#include "arm_math.h"
#include "math.h"

#include "sDBG_Debug.h"

#include "FreeRTOS.h"
#include "task.h"


#include "sG2D.hpp"


/*APP*/
#include "sAPP_IMU.hpp"
#include "sAPP_Car.hpp"
#include "sAPP_Btns.h"
#include "sAPP_Motor.hpp"
#include "sAPP_BlcCtrl.hpp"
#include "sAPP_Tasks.hpp"
#include "sAPP_AHRS.hpp"
#include "sAPP_BlcCtrl.hpp"

/*MID*/
// #include "sGraphic2D.h"
#include "sDRV_GenOLED.h"
#include "cm_backtrace.h"


/*DBG*/
#include "sDBG_UART.hpp"


/*LIB*/
#include "sLib.h"

/*DRV*/
#include "sDRV_ICM42688.h"
#include "sDRV_LIS3MDLTR.h"
#include "sDRV_DRV8870.h"
#include "sDRV_GMR.h"
#include "sDRV_PwrLight.h"
#include "sDRV_MB85RCxx.h"
#include "sDRV_TrackTube8.h"
#include "sDRV_INA219.h"
#include "sDRV_LED.hpp"
#include "sDRV_PS2.h"
#include "PS2.h"

/*BSP*/
#include "sBSP_DWT.h"
#include "sBSP_RCC.h"
#include "sBSP_SYS.h"
#include "sBSP_SPI.h"
#include "sBSP_TIM.h"
#include "sBSP_I2C.h"
#include "sBSP_ADC.h"
#include "sBSP_UART.h"
#include "sBSP_DMA.h"


#define APPNAME                        "GCARCv4"
#define HARDWARE_VERSION               "V4"
#define SOFTWARE_VERSION               "V1.0"




