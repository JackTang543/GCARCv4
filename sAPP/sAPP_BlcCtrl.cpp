#include "sAPP_BlcCtrl.hpp"


/*这些是上电默认值*/
//机械中值
#define MECHINE_CENTER_ANGLE    (  -1.2f)
//直立环初始值
#define STAND_KP                (-12.00f * 0.6f)
#define STAND_KD                (- 0.60f * 0.6f)
//速度环初始值
#define SPD_KP                  ( 0.30f)
#define SPD_KI                  (SPD_KP / 200.0f)
//转向环初始值
#define TURN_KP                 (  0.20f)
#define TURN_KD                 (  0.05f)
//0速度环初始值
#define ZERO_KP                 (200.00f)
#define ZERO_KI                 ( 80.00f)
#define ZERO_KD                 (  0.00f)

//轮胎半径
#define WHEEL_RADIUS            (0.0672f)


sAPP_BlcCtrl_Blc_t g_blc;
sAPP_BlcCtrl_Ctrl_t g_ctrl;
//保存导航信息
sAPP_CTRL_Nav_t g_nav;

sLib_IncPID_t pid_inc_pos;

//PD直立环
static inline float StandPDCtrler(float pitch_deg,float pitch_deg_spd);
//PI速度环
static float SpdPICtrler(float left_rpm,float right_rpm);
//PD转向环
static float TurnPDCtrler(float gyro_z);


//平衡控制初始化
void sAPP_BlcCtrl_Init(){
    //设置各个参数的初始值
    g_blc.m_angle = MECHINE_CENTER_ANGLE;
    g_blc.stand_kp = STAND_KP;
    g_blc.stand_kd = STAND_KD;
    g_blc.spd_kp   = SPD_KP;
    g_blc.spd_ki   = SPD_KI;
    g_blc.turn_kd  = TURN_KD;
    g_blc.turn_kp  = TURN_KP;

    g_blc.inc_pos_kp = ZERO_KP;
    g_blc.inc_pos_ki = ZERO_KI;
    g_blc.inc_pos_kd = ZERO_KD;

    //上电禁用平衡
    g_ctrl.blc_en = 0;

    sLib_IncPIDInit(&pid_inc_pos,-200,200,1000,1000);
    sLib_IncPIDSetK(&pid_inc_pos,g_blc.inc_pos_kp, g_blc.inc_pos_ki, g_blc.inc_pos_kd);
    sLib_IncPIDSetTarget(&pid_inc_pos,0);
}

#include "sDRV_PS2.h"
#include "sBSP_UART.h"

void sAPP_BlcCtrl_Handler(){
    #define DT_S (0.010f)    //10ms 100Hz控制周期

    sDRV_PS2_Handler();

    static float input_spd,input_head;
    input_spd = -(float)(ps2.leftY - 0x7F);
    input_head = -(float)(ps2.rightX - 0x7F);

    input_head = sLib_Fmap(input_head,-128,127,-180,180);

    if(input_head < 5 && input_head > -5){
        input_head = 0;
    }

    g_ctrl.tar_spd = sLib_Fmap(input_spd,-128,127,-200,200);


    g_ctrl.tar_head += input_head * DT_S;
    if(g_ctrl.tar_head > 180){
        g_ctrl.tar_head -= 360;
    }
    if(g_ctrl.tar_head < -180){
        g_ctrl.tar_head += 360;
    }
    

    if(g_ctrl.tar_spd < 5 && g_ctrl.tar_spd > -5){
        g_ctrl.tar_spd = 0;
    }

    if(g_ctrl.tar_spd < 5 && g_ctrl.tar_spd > -5){
        g_ctrl.tar_spd = 0;
    }

    


    //sBSP_UART_Debug_Printf("tar_spd: %6.1f,tar_head:%6.1f \n",g_ctrl.tar_spd,g_ctrl.tar_head);

    //如果没有启用平衡就不计算
    if(g_ctrl.blc_en == 0){
        //电机设置开路
        motor.setLBrake();
        motor.setRBrake();
        goto PRINT;
        //return;
    }

    

    

    if(ahrs.pitch > 45 || ahrs.pitch < -45){
        sDRV_DRV8870_SetRightBrake(1);
        sDRV_DRV8870_SetLeftBrake(1);
        //关闭平衡开关
        g_ctrl.blc_en = 0;
        //Error_Handler();
    }

    //! 负号
    g_blc.left_rpm  = motor.getLRPM();
    g_blc.right_rpm = motor.getRRPM();

    

    float inc_pos_out;

    //应用转向环
    g_blc.turn_out  = TurnPDCtrler (ahrs.yaw);

    if(fabs(g_blc.turn_out) < 10){
        //对编码器读到的转速值转换成线速度并积分
        g_nav.y_pos += (g_blc.left_rpm + g_blc.right_rpm) * ((2.0f * 3.1416 * WHEEL_RADIUS) / 60.0f) * DT_S;
    }

    if(g_ctrl.tar_spd < 1 && g_ctrl.tar_spd > -1){
        inc_pos_out = sLib_IncPIDUpdate(&pid_inc_pos,g_nav.y_pos,DT_S);
        g_ctrl.tar_spd = inc_pos_out;
    }else{
        g_nav.y_pos = 0;
        pid_inc_pos.integral = 0;
        inc_pos_out = 0;
    }

    //计算直立环
    g_blc.stand_out = StandPDCtrler(ahrs.pitch,ahrs.gyr_x);
    //计算速度环
    g_blc.spd_out   = SpdPICtrler  (g_blc.left_rpm,g_blc.right_rpm);
    

    //合并三环输出
    g_blc.left_pwm  = g_blc.stand_out + g_blc.spd_out + g_blc.turn_out;
    g_blc.right_pwm = g_blc.stand_out + g_blc.spd_out - g_blc.turn_out;


    //输出限幅
    sLib_FLimit(&g_blc.left_pwm,-100,100);
    sLib_FLimit(&g_blc.right_pwm,-100,100);
    
    motor.setLM(-g_blc.left_pwm);
    motor.setRM(-g_blc.right_pwm);



    PRINT:
    (void)0;

    //调试:平衡
    //dbg.printf("%.2f,%.2f,%.2f,%.2f,%.4f,%.2f\n",ahrs.pitch,ahrs.gyr_x,g_blc.left_pwm,g_blc.right_pwm,motor.getLRPM(),motor.getRRPM());
    //dbg.printf("%.2f,%.2f,%.2f,%.2f,%.4f,%.2f\n",ahrs.pitch,g_blc.left_pwm,g_blc.right_pwm,motor.getLRPM(),motor.getRRPM());


}




//直立环PD控制,传参:俯仰角,俯仰角速度
static inline float StandPDCtrler(float pitch_deg,float pitch_deg_spd){
    //应用机械中值后,使用直立环PD控制器
    return g_blc.stand_kp * (pitch_deg - g_blc.m_angle) + g_blc.stand_kd * pitch_deg_spd;
}

//速度环PI控制,传参:左右电机编码器转速
static float SpdPICtrler(float left_rpm,float right_rpm){
    static float velocity, Encoder_Least, Encoder_bias;
    static float Encoder_Integral;

    //计算最新速度偏差 = 目标速度- 测量速度（左右编码器之和）
    Encoder_Least = g_ctrl.tar_spd - (left_rpm + right_rpm);
    //一阶低通滤波器,减缓速度变化
    Encoder_bias *= 0.86;
    Encoder_bias += Encoder_Least * 0.14;
    //积分出位移,积分时间：10ms
    Encoder_Integral += Encoder_bias;
    //接收遥控器数据,控制前进后退
    Encoder_Integral += g_ctrl.tar_move;
    //积分限幅
    sLib_FLimit(&Encoder_Integral, -1000, 1000);
    //计算速度PI
    velocity = -Encoder_bias * g_blc.spd_kp - Encoder_Integral * g_blc.spd_ki;
    return velocity;
}


float angleDifference(float angle1, float angle2) {
    float diff = angle2 - angle1;
    while (diff < -180.0) diff += 360.0;
    while (diff > 180.0) diff -= 360.0;
    return diff;
}

//转向环PD控制,传参:陀螺仪角度
static float TurnPDCtrler(float gyro_z){
    //转向值,用于返回
    static float turn_val;

    float error = angleDifference(g_ctrl.tar_head, gyro_z);

    //计算转向PD控制器
    //turn_val = (gyro_z - ctrl.tar_head) * blc.turn_kp + (gyro_z - ctrl.tar_head) * blc.turn_kd;

    turn_val = error * g_blc.turn_kp + error * g_blc.turn_kd;

    return turn_val;
}



