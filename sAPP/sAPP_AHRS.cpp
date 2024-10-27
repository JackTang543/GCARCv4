#include "sAPP_AHRS.hpp"


#include "MadgwickAHRS.h"

/*sAPP_AHRS.cpp 用于姿态估计*/
#include "main.h"

sAPP_AHRS ahrs;



int sAPP_AHRS::init(){
    
    return 0;
}


float bias_acc_x ;
float bias_acc_y ;
float bias_acc_z ;
float bias_gyro_x;
float bias_gyro_y;
float bias_gyro_z;

int sAPP_AHRS::calcBias(){
    #define POINT_COUNT 1000


    HAL_Delay(1000);
    

	float acc_x_accu = 0;
	float acc_y_accu = 0;
	float acc_z_accu = 0;
	float gyro_x_accu = 0;
	float gyro_y_accu = 0;
	float gyro_z_accu = 0;
	for(uint16_t i = 0; i < POINT_COUNT; i++){
		//更新IMU采集的数据
        imu.update();
		acc_x_accu  += imu.acc_x;
		acc_y_accu  += imu.acc_y;
		acc_z_accu  += imu.acc_z;
		gyro_x_accu += imu.gyr_x;
		gyro_y_accu += imu.gyr_y;
		gyro_z_accu += imu.gyr_z;
		HAL_Delay(1);
	}
	bias_acc_x  = acc_x_accu  / POINT_COUNT;
	bias_acc_y  = acc_y_accu  / POINT_COUNT;
	bias_acc_z  = acc_z_accu  / POINT_COUNT - 9.81398f;	//重力加速度
	bias_gyro_x = gyro_x_accu / POINT_COUNT;
	bias_gyro_y = gyro_y_accu / POINT_COUNT;
	bias_gyro_z = gyro_z_accu / POINT_COUNT;

    return 0;
}


sLIB_6AXIS_INPUT_t input;
sLIB_ATTITUDE_RESULT_t result;

int sAPP_AHRS::update(){
    //更新IMU采集的数据
    imu.update();

    input.acc_x = imu.acc_x - bias_acc_x;
    input.acc_y = imu.acc_y - bias_acc_y;
    input.acc_z = imu.acc_z - bias_acc_z;
    input.gyro_x = imu.gyr_x - bias_gyro_x;
    input.gyro_y = imu.gyr_y - bias_gyro_y;
    input.gyro_z = imu.gyr_z - bias_gyro_z;

    //融合算法
    sLib_6AxisCompFilter(&input, &result);

    ahrs.acc_x = imu.acc_x;
    ahrs.acc_y = imu.acc_y;
    ahrs.acc_z = imu.acc_z;
    ahrs.gyr_x = imu.gyr_x;
    ahrs.gyr_y = imu.gyr_y;
    ahrs.gyr_z = imu.gyr_z;

    ahrs.pitch = result.pitch;
    ahrs.roll = result.roll;
    ahrs.yaw = result.yaw;
    ahrs.q0 = result.q0;
    ahrs.q1 = result.q1;
    ahrs.q2 = result.q2;
    ahrs.q3 = result.q3;

    //dbg.printf("pitch: %6.2f, roll: %6.2f, yaw: %6.2f\n",ahrs.pitch,ahrs.roll,ahrs.yaw);

    return 0;
}








