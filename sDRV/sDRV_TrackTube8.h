#pragma once
#ifdef __cplusplus
extern "C"{
#endif



#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "sBSP_SPI.h"


typedef enum{
    SDRV_TRACKTUBE8_ALLOFF,      //所有光电管全是灭的
    SDRV_TRACKTUBE8_ALLON,       //所有光电管全是亮的
    SDRV_TRACKTUBE8_2PATH,       //检测到两条路径
    SDRV_TRACKTUBE8_VALID,       //数据有效(一条路径)
    SDRV_TRACKTUBE8_INVALID      //数据无效
}sDRV_TrackTube8_Status_t;


void sDRV_TrackTube8_Init();
sDRV_TrackTube8_Status_t sDRV_TrackTube8_GetData(float* pPath1,float* pPath2);


#ifdef __cplusplus
}
#endif
