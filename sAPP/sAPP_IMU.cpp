#include "sAPP_IMU.hpp"


sAPP_IMU imu;

#include "sGCARCv4_Def.h"
#include "sBSP_SPI.h"


sAPP_IMU::sAPP_IMU(){

}

sAPP_IMU::~sAPP_IMU(){
    
}


int sAPP_IMU::init(){
    this->icm  = &g_icm;
    this->lis3 = &g_lis3;

    /*把IMU的2个CS都上拉*/
    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = ICM_CS_Pin | LIS3_CS_Pin;
    HAL_GPIO_Init(GPIOC,&gpio);
    HAL_GPIO_WritePin(GPIOC,ICM_CS_Pin ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC,LIS3_CS_Pin,GPIO_PIN_SET);

    /*SPI2 <-> IMU_SPI*/
    sBSP_SPI_IMU_Init(SPI_BAUDRATEPRESCALER_4);   //11.25MBits/s


    if(sDRV_ICM_Init() != 0){
        return -1;
    }

    if(sDRV_LIS3_Init() != 0){
        return -2;
    }


    return 0;
}


int sAPP_IMU::update(){
    sDRV_ICM_GetData();
    sDRV_LIS3_GetData();


    acc_x = icm->acc_x;
    acc_y = icm->acc_y;
    acc_z = icm->acc_z;
    gyr_x = icm->gyro_x;
    gyr_y = icm->gyro_y;
    gyr_z = icm->gyro_z;
    mag_x = lis3->mag_x;
    mag_y = lis3->mag_y;
    mag_z = lis3->mag_z;
    icm_temp = icm->temp;
    lis3_temp = lis3->temp;
    tick = icm->tick;


    return 0;
}


