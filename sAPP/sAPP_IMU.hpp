#pragma once

#include "stm32f4xx_hal.h"

#include "sDRV_ICM42688.h"
#include "sDRV_LIS3MDLTR.h"



typedef struct{
    float gyr_x,gyr_y,gyr_z;
    float acc_x,acc_y,acc_z;
    float mag_x,mag_y,mag_z;
    float icm_temp,lis3_temp,tick;  //这个tick是ICM的tick
}IMU_Data_t;




class sAPP_IMU{
public:
    sAPP_IMU();
    ~sAPP_IMU();

    float gyr_x,gyr_y,gyr_z;
    float acc_x,acc_y,acc_z;
    float mag_x,mag_y,mag_z;
    float icm_temp,lis3_temp,tick;  //这个tick是ICM的tick

    int init();

    int update();


private:

    sDRV_ICM_Data_t*  icm;
    sDRV_LIS3_Data_t* lis3;





};


extern sAPP_IMU imu;



