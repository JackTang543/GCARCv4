#pragma once


#include "stm32f4xx_hal.h"

#include "sGCARCv4_hal_msp.h"
#include "sGCARCv4_PinDefine.h"

#include "arm_math.h"
#include "math.h"

#include "sDBG_Debug.h"




/*APP*/
#include "sAPP_Btns.h"
#include "sAPP_PMDC.h"

/*MID*/
#include "sGraphic2D.h"
#include "sDRV_GenOLED.h"


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

/*BSP*/
#include "sBSP_DWT.h"
#include "sBSP_RCC.h"
#include "sBSP_SYS.h"
#include "sBSP_SPI.h"
#include "sBSP_TIM.h"
#include "sBSP_I2C.h"
#include "sBSP_ADC.h"




extern sDBG_UART dbg;
