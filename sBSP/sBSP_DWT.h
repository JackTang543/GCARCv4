#ifndef __SBSP_DWT_H__
#define __SBSP_DWT_H__
//控制是否启用这个模块
#define __SBSP_DWT_EN__
#ifdef  __SBSP_DWT_EN__

#ifdef __cplusplus
extern "C"{
#endif

//sBSP_DWT.h
//By Sightseer. 2023.12.30 v1.0

/* 使用说明:
* 1.不适用于Cortex-M0/M0+内核,他们不具有DWT单元,测试平台:STM32F103HAL库
* 2.首先使用sBSP_DWT_Init(HAL_RCC_GetSysClockFreq());初始化外设
* 3.如果你想使用DWT做us/ms级延时,使用sBSP_DWT_Delay_us(us);或者sBSP_DWT_Delay_ms(ms);
* 4.如果你想用DWT做us级时间测量,使用sBSP_DWT_MeasureStart();开始测量,使用sBSP_DWT_MeasureEnd();结束测量
* 5.测量结束后,使用sBSP_DWT_GetMeasure_us();获取测量结果(us)
*/

#include "stm32f4xx_hal.h"

void sBSP_DWT_Init(uint32_t freq);
void sBSP_DWT_Delay_us(uint32_t time_us);
void sBSP_DWT_Delay_ms(uint32_t time_ms);
void sBSP_DWT_MeasureStart();
void sBSP_DWT_MeasureEnd();
uint32_t sBSP_DWT_GetMeasure_us();

#ifdef __cplusplus
}
#endif
#endif
#endif
