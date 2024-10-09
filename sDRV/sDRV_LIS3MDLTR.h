#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"

//X,Y轴操作模式选择 应用位段:MSK_CTRLREG1_OM
typedef enum{
    SDRV_LIS3_XYOM_LP                 = 0b00,       //低功耗模式
    SDRV_LIS3_XYOM_MP                 = 0b01,       //中性能模式
    SDRV_LIS3_XYOM_HP                 = 0b10,       //高性能模式
    SDRV_LIS3_XYOM_UHP                = 0b11,       //超高性能模式
}sDRV_LIS3_XYOM_t;

//Z轴操作模式选择 应用位段:MSK_CTRLREG4_OMZ
typedef enum{
    SDRV_LIS3_ZOM_LP                  = 0b00,       //低功耗模式
    SDRV_LIS3_ZOM_MP                  = 0b01,       //中性能模式
    SDRV_LIS3_ZOM_HP                  = 0b10,       //高性能模式
    SDRV_LIS3_ZOM_UHP                 = 0b11,       //超高性能模式
}sDRV_LIS3_ZOM_t;

//ODR 应用位段:MSK_CTRLREG1_DO
typedef enum{
    SDRV_LIS3_ODR_0D625HZ             = 0b000,
    SDRV_LIS3_ODR_1D25HZ              = 0b001,
    SDRV_LIS3_ODR_2D5HZ               = 0b010,
    SDRV_LIS3_ODR_5HZ                 = 0b011,
    SDRV_LIS3_ODR_10HZ                = 0b100,
    SDRV_LIS3_ODR_20HZ                = 0b101,
    SDRV_LIS3_ODR_40HZ                = 0b110,
    SDRV_LIS3_ODR_80HZ                = 0b111,
}sDRV_LIS3_ODR_t;

//量程配置 应用位段:MSK_CTRLREG2_FS
typedef enum{
    SDRV_LIS3_FS_4GAUSS               = 0b00,
    SDRV_LIS3_FS_8GAUSS               = 0b01,
    SDRV_LIS3_FS_12GAUSS              = 0b10,
    SDRV_LIS3_FS_16GAUSS              = 0b11,
}sDRV_LIS3_FS_t;

//操作模式 应用位段:MSK_CTRLREG3_MD
typedef enum{
    SDRV_LIS3_MD_CONTINUOUS           = 0b00,
    SDRV_LIS3_MD_SINGLE               = 0b01,
    SDRV_LIS3_MD_OFF                  = 0b10,
}sDRV_LIS3_MD_t;


typedef struct{
    float mag_x;    //单位mGa
    float mag_y;
    float mag_z;
    float temp;
    sDRV_LIS3_FS_t fs;
}sDRV_LIS3_Data_t;




extern sDRV_LIS3_Data_t g_lis3;



int sDRV_LIS3_Init();
void sDRV_LIS3_GetData();


#ifdef __cplusplus
}
#endif


