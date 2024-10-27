#pragma once
#ifdef __cplusplus
    extern "C" {
#endif

#include "sLib.h"

typedef struct{
    float Kp;           //比例增益
    float Ki;           //积分增益
    float Kd;           //微分增益
    float setpoint;     //设定目标值
    float integral;     //积分累计
    float prev_error;   //上一次的误差
    float output_min;   //输出限制最小值
    float output_max;   //输出限制最大值
    float integral_max; //积分限幅最大值
    float integral_min; //积分限幅最小值
}sLib_PosPID_t;

typedef struct{
    float Kp;           //比例增益
    float Ki;           //积分增益
    float Kd;           //微分增益
    float setpoint;     //设定目标值
    float integral;     //积分累计
    float prev_error;   //上一次的误差
    float delta_output; //输出的变化量
    float output;       //当前输出值
    float output_min;   //输出限制最小值
    float output_max;   //输出限制最大值
    float integral_max; //积分限幅最大值
    float integral_min; //积分限幅最小值
}sLib_IncPID_t;



/*适用于SISO系统的一般式PID控制器*/

/*位置式PID*/
void  sLib_PosPIDInit     (sLib_PosPID_t* pid,float out_min,float out_max,float int_min,float int_max);
void  sLib_PosPIDSetK     (sLib_PosPID_t* pid,float Kp,float Ki,float Kd);
void  sLib_PosPIDSetTarget(sLib_PosPID_t* pid,float setpoint);
float sLib_PosPIDGetTarget(sLib_PosPID_t* pid);
float sLib_PosPIDUpdate   (sLib_PosPID_t* pid, float measured_value, float dt_s);

/*增量式PID*/
void  sLib_IncPIDInit     (sLib_IncPID_t* pid,float out_min,float out_max,float int_min,float int_max);
void  sLib_IncPIDSetK     (sLib_IncPID_t* pid,float Kp,float Ki,float Kd);
void  sLib_IncPIDSetTarget(sLib_IncPID_t* pid,float setpoint);
float sLib_IncPIDGetTarget(sLib_IncPID_t* pid);
float sLib_IncPIDUpdate   (sLib_IncPID_t* pid, float measured_value, float dt_s);





#ifdef __cplusplus
    }
#endif

