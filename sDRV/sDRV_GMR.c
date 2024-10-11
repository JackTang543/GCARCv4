#include "sDRV_GMR.h"

#include "sBSP_TIM.h"

//TIM3 CH1 CH2 -> Left电机
//TIM4 CH1 CH2 -> Right电机

//500线GMR转一圈30K个脉冲
//好像不能使用双边沿模式,也就是四倍频模式

//241011 PM05:24
//新增功能，在Handler里，自适应时间间隔，使用HAL_GetTick，原来是define的固定间隔，如果调用时间不一致就会数据不对
//但是还是得注意数据溢出问题


#define GMR_ROUND_PLUSE     (30000)


float right_rpm;
float left_rpm;

void sDRV_GMR_Init(){
    sBSP_TIM_GMRL_Init();
    sBSP_TIM_GMRL_SetEN(1);
    sBSP_TIM_GMRL_Set(30000);

    sBSP_TIM_GMRR_Init();
    sBSP_TIM_GMRR_SetEN(1);
    sBSP_TIM_GMRR_Set(30000);
}


float sDRV_GMR_GetLeftRPM(){
    //! 两个电机方向是相反的
    return -left_rpm;
}

float sDRV_GMR_GetRightRPM(){
    return right_rpm;
}

//10ms一次
void sDRV_GMR_Handler(){
    static uint32_t prev_time;
    static uint32_t now_time;
    static float dtms;
    prev_time = now_time;
    now_time = HAL_GetTick();
    dtms = (now_time - prev_time);

    //Right
    right_rpm = (float)sBSP_TIM_GMRR_Get() - 30000.0f;
    right_rpm *= (1000.0f / dtms);
    right_rpm = right_rpm / GMR_ROUND_PLUSE * 60 * 0.5;
    sBSP_TIM_GMRR_Set(30000);

    //Left
    left_rpm = (float)sBSP_TIM_GMRL_Get() - 30000.0f;
    left_rpm *= (1000.0f / dtms);
    left_rpm = left_rpm / GMR_ROUND_PLUSE * 60 * 0.5;
    sBSP_TIM_GMRL_Set(30000);

}




