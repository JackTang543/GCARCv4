#include "sLib_Ctrler.h"




//初始化PID控制器,需要再设置初始K值和目标
void sLib_PosPIDInit(sLib_PosPID_t* pid,float out_min,float out_max,float int_min,float int_max){
    //使用设置K值函数设置K
    pid->Kp = 0.0f;
    pid->Ki = 0.0f;
    pid->Kd = 0.0f;
    //使用设置Target函数设置目标点
    pid->setpoint = 0.0f;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->integral_max = int_max;
    pid->integral_min = int_min;
}


void inline sLib_PosPIDSetK(sLib_PosPID_t* pid,float Kp,float Ki,float Kd){
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}


void inline sLib_PosPIDSetTarget(sLib_PosPID_t* pid,float setpoint){
    pid->setpoint = setpoint;
    //pid->integral = 0.0f;
    //pid->prev_error = 0.0f;
}

float inline sLib_PosPIDGetTarget(sLib_PosPID_t* pid){
    return pid->setpoint;
}

//更新PID控制器
float sLib_PosPIDUpdate(sLib_PosPID_t* pid, float measured_value, float dt_s){
    //计算误差
    float error = pid->setpoint - measured_value;
    //计算新的积分
    pid->integral = pid->integral + error * dt_s;
    //积分限幅
    sLib_FLimit(&pid->integral, pid->integral_min, pid->integral_max);
    //计算微分
    float derivative = (error - pid->prev_error) / dt_s;
    //计算PID输出
    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    //保存当前误差为下一次计算的"上一次误差"
    pid->prev_error = error;
    //限制输出范围
    sLib_FLimit(&output, pid->output_min, pid->output_max);
    return output;
}


//初始化增量式PID控制器
void sLib_IncPIDInit(sLib_IncPID_t* pid,float out_min,float out_max,float int_min,float int_max){
    pid->Kp = 0.0f;
    pid->Ki = 0.0f;
    pid->Kd = 0.0f;
    pid->setpoint = 0.0f;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->output = 0.0f; //初始化输出值
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->integral_max = int_max;
    pid->integral_min = int_min;
}


void inline sLib_IncPIDSetK(sLib_IncPID_t* pid,float Kp,float Ki,float Kd){
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}

void inline sLib_IncPIDSetTarget(sLib_IncPID_t* pid,float setpoint){
    pid->setpoint = setpoint;
}

float inline sLib_IncPIDGetTarget(sLib_IncPID_t* pid){
    return pid->setpoint;
}

float sLib_IncPIDUpdate(sLib_IncPID_t* pid, float measured_value, float dt_s){
    //计算误差
    float error = pid->setpoint - measured_value;
    //计算误差变化
    float delta_error = error - pid->prev_error;
    //计算新的积分
    pid->integral = pid->integral + error * dt_s;
    //积分限幅
    sLib_FLimit(&pid->integral, pid->integral_min, pid->integral_max);
    //计算微分
    float derivative = delta_error / dt_s;
    //计算增量输出
    pid->delta_output = (pid->Kp * delta_error) + (pid->Ki * error * dt_s) + (pid->Kd * derivative);
    //更新当前输出
    pid->output += pid->delta_output;
    //限制输出范围
    sLib_FLimit(&pid->output,pid->output_min,pid->output_max);
    //保存当前误差为下一次计算的"上一次误差"
    pid->prev_error = error;
    return pid->output;
}

