#pragma once


//#include "main.h"
#include "sLib.h"
#include "sAPP_Motor.hpp"
#include "sAPP_AHRS.hpp"


//小车平衡参数
typedef struct{
    float stand_kp;     //直立环Kp
    float stand_kd;     //直立环Kd
    float spd_kp;       //速度环Kp
    float spd_ki;       //速度环Ki = Kp / 200
    float turn_kp;      //转向环Kp
    float turn_kd;      //转向环Kd

    float m_angle;      //机械中值,单位deg

    float left_pwm;     //左轮PWM,单位百分比,-100~100
    float right_pwm;    //右轮PWM
    float left_rpm;     //左轮编码器转速,单位RPM
    float right_rpm;    //右轮编码器转速
    
    float stand_out;    //直立环输出
    float spd_out;      //速度环输出
    float turn_out;     //转向环输出

    float inc_pos_kp;
    float inc_pos_ki;
    float inc_pos_kd;
}sAPP_BlcCtrl_Blc_t;


//小车控制参数
typedef struct{
    float tar_spd;      //目标速度
    float tar_head;     //目标航向
    float tar_move;     //目标移动

    float tar_spd2;

    float turn_spd;     //转向速度
    
    bool  blc_en;       //平衡使能
}sAPP_BlcCtrl_Ctrl_t;

//导航参数
typedef struct{
    float x_acc_pure;   //去除重力分量的纯加速度
    float y_acc_pure;
    float z_acc_pure;
    float x_spd;        //速度
    float y_spd;
    float x_pos;        //位置
    float y_pos;

} sAPP_CTRL_Nav_t;


extern sAPP_BlcCtrl_Blc_t g_blc;
extern sAPP_BlcCtrl_Ctrl_t g_ctrl;


void sAPP_BlcCtrl_Init();
void sAPP_BlcCtrl_Handler();



