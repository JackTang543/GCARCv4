#pragma once
#ifdef __cplusplus
    extern "C" {
#endif

#if defined(STM32F103xB) || defined(STM32F103xE)
    #include "stm32f1xx_hal.h"
#endif
#if defined(STM32F411xE) || defined(STM32F446xE)
    #include "stm32f4xx_hal.h"
#endif

#include "stm32f4xx_hal.h"


#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>

#include "sLib_Ctrler.h"
#include "sLib_Filter.h"
#include "sLib_Reg.h"


//sightseer's library
//一个用于嵌入式的常用算法和函数库


//v1.0 240719
//初版

//使用哪种malloc和free
#define sLib_PortMalloc malloc
#define sLib_PortFree   free
// #define sLib_PortMalloc pvPortMalloc
// #define sLib_PortFree   pvPortFree






//PI
#define M_PI            (3.14159f)
//EXP
#define M_EXP           (2.71828f)

//重力常数
#define M_GRAVITY       (9.81398f)

//度转弧度
#define DEG2RAD		0.017453f
//弧度转度
#define RAD2DEG		57.29578f


/*常用函数*/

//把一个浮点数从一个范围转换到另一个范围
float sLib_Fmap(float x, float in_min, float in_max, float out_min, float out_max);
//把一个整数从一个范围转换到另一个范围
uint32_t sLib_Imap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
//快速求平方根倒数
float sLib_InvSqrt(float x);
//从气压转换到高度,返回单位m
float sLib_Press2Alt(float press_pa);
//比较两个浮点数的大小,可以用于qsort的比较函数,返回1: a>b, 0: a==b, -1: a<b
int sLib_FCmp(const void *a, const void *b);
//比较两个浮点数是否相等
int sLib_FIsEqual(float fa, float fb);
//获得航向角的差值
float sLib_GetHeadingDiff(float heading1, float heading2);
//浮点数限幅
void sLib_FLimit(float *val, float min, float max);
//伽马矫正
float sLib_GammaCorrect(float percent_brightness);



#ifdef __cplusplus
    }
#endif

