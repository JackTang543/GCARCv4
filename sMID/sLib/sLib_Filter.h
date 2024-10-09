#pragma once
#ifdef __cplusplus
    extern "C" {
#endif


#include "sLib.h"

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>


//六轴数据输入
typedef struct{
    float acc_x;
    float acc_y;
    float acc_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
}sLIB_6AXIS_INPUT_t;

//姿态解算结果
typedef struct{
    float pitch;
    float roll;
    float yaw;
    float q0;
    float q1;
    float q2;
    float q3;
}sLIB_ATTITUDE_RESULT_t;

//SWF滑窗滤波
typedef struct {
    uint8_t size;       //窗口大小
    float*  data;       //数据缓冲区
    uint8_t index;      //当前数据索引
    float   sum;        //数据的总和，用于快速计算平均值
    float   average;    //当前的平均值
}sLIB_SWF_t;

//MWF中值滤波
typedef struct {
    uint8_t size;     //窗口大小
    float*  data;     //数据缓冲区
    uint8_t index;    //当前数据索引
}sLIB_MWF_t;

//一阶指数移动平均滤波
typedef struct {
    float alpha;     // 平滑因子,决定了新数据在平均值中的权重
    float lastValue; // 上一次的滤波结果
}sLib_EMA1_t;


//六轴姿态互补滤波
void sLib_6AxisCompFilter(sLIB_6AXIS_INPUT_t* input,sLIB_ATTITUDE_RESULT_t* result);

//滑窗滤波
void sLib_SWFInit(sLIB_SWF_t* pFliter,uint8_t win_size);
void sLib_SWFDeInit(sLIB_SWF_t* pFliter);
float sLib_SWFUpdate(sLIB_SWF_t* pFliter,float newData);

//中值滤波
void sLib_MWFInit(sLIB_MWF_t* pFilter, uint8_t size);
float sLib_MWFUpdate(sLIB_MWF_t* pFilter, float newData);

//一阶指数移动平均滤波
void sLib_EMA1Init(sLib_EMA1_t* pFilter, float alpha);
float sLib_EMA1Update(sLib_EMA1_t* pFilter, float newData);

#ifdef __cplusplus
    }
#endif

