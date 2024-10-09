#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"

//读出的数据
typedef struct{
    float    temp;         //温度,单位摄氏度
    float    acc_x;        //加速度数据,单位m/s^2
    float    acc_y;
    float    acc_z;
    float    gyro_x;       //陀螺仪数据,单位dps
    float    gyro_y;
    float    gyro_z;
    uint32_t tick;         //
}sDRV_ICM_Data_t;



//陀螺仪量程
typedef enum{
    SDRV_ICM_GYROFS_2000DPS   = 0b000,
    SDRV_ICM_GYROFS_1000DPS   = 0b001,
    SDRV_ICM_GYROFS_500DPS    = 0b010,
    SDRV_ICM_GYROFS_250DPS    = 0b011,
    SDRV_ICM_GYROFS_125DPS    = 0b100,
    SDRV_ICM_GYROFS_62D5DPS   = 0b101,
    SDRV_ICM_GYROFS_31D25DPS  = 0b110,
    SDRV_ICM_GYROFS_15D625DPS = 0b111,
}sDRV_ICM_GYROFS_t;

//陀螺仪ODR
typedef enum{
    SDRV_ICM_GYROODR_32KHZ    = 0b0001,
    SDRV_ICM_GYROODR_16KHZ    = 0b0010,
    SDRV_ICM_GYROODR_8KHZ     = 0b0011,
    SDRV_ICM_GYROODR_4KHZ     = 0b0100,
    SDRV_ICM_GYROODR_2KHZ     = 0b0101,
    SDRV_ICM_GYROODR_1KHZ     = 0b0110,
    SDRV_ICM_GYROODR_200HZ    = 0b0111,
    SDRV_ICM_GYROODR_100HZ    = 0b1000,
    SDRV_ICM_GYROODR_50HZ     = 0b1001,
    SDRV_ICM_GYROODR_25HZ     = 0b1010,
    SDRV_ICM_GYROODR_12D5HZ   = 0b1011,
    SDRV_ICM_GYROODR_500HZ    = 0b1111,
}sDRV_ICM_GYROODR_t;

//加速度计满量程配置
typedef enum{
    SDRV_ICM_ACCELFS_16G      = 0b000,
    SDRV_ICM_ACCELFS_8G       = 0b001,
    SDRV_ICM_ACCELFS_4G       = 0b010,
    SDRV_ICM_ACCELFS_2G       = 0b011,
}sDRV_ICM_ACCELFS_t;

//加速度计ODR
typedef enum{
    SDRV_ICM_ACCELODR_32KHZ    = 0b0001,    //LN
    SDRV_ICM_ACCELODR_16KHZ    = 0b0010,    //LN
    SDRV_ICM_ACCELODR_8KHZ     = 0b0011,    //LN
    SDRV_ICM_ACCELODR_4KHZ     = 0b0100,    //LN
    SDRV_ICM_ACCELODR_2KHZ     = 0b0101,    //LN
    SDRV_ICM_ACCELODR_1KHZ     = 0b0110,    //LN(默认)
    SDRV_ICM_ACCELODR_200HZ    = 0b0111,    //LP/LN
    SDRV_ICM_ACCELODR_100HZ    = 0b1000,    //LP/LN
    SDRV_ICM_ACCELODR_50HZ     = 0b1001,    //LP/LN
    SDRV_ICM_ACCELODR_25HZ     = 0b1010,    //LP/LN
    SDRV_ICM_ACCELODR_12D5HZ   = 0b1011,    //LP/LN
    SDRV_ICM_ACCELODR_6D25HZ   = 0b1100,    //LP
    SDRV_ICM_ACCELODR_3D125HZ  = 0b1101,    //LP
    SDRV_ICM_ACCELODR_1D5625HZ = 0b1110,    //LP
    SDRV_ICM_ACCELODR_500HZ    = 0b1111,    //LP/LN  
}sDRV_ICM_ACCELODR_t;

//温度计DLPF设置
typedef enum{
    SDRV_ICM_TEMPFILTBW_4000HZ = 0b000,     //BW=4KHZ 延迟0.125ms(默认)
    SDRV_ICM_TEMPFILTBW_170HZ  = 0b001,     //BW=170HZ 1ms
    SDRV_ICM_TEMPFILTBW_82HZ   = 0b010,     //BW=82HZ 2ms
    SDRV_ICM_TEMPFILTBW_40HZ   = 0b011,     //BW=40HZ 4ms
    SDRV_ICM_TEMPFILTBW_20HZ   = 0b100,     //BW=20HZ 8ms
    SDRV_ICM_TEMPFILTBW_10HZ   = 0b101,     //BW=10HZ 16ms
    SDRV_ICM_TEMPFILTBW_5HZ    = 0b110,     //BW=5HZ  32ms
}sDRV_ICM_TEMPFILTBW_t;

//陀螺仪滤波阶数
typedef enum{
    SDRV_ICM_GYROUIFILTORD_1   = 0b00,      //1阶滤波器
    SDRV_ICM_GYROUIFILTORD_2   = 0b01,      //2阶滤波器
    SDRV_ICM_GYROUIFILTORD_3   = 0b10,      //3阶滤波器
}sDRV_ICM_GYROUIFILTORD_t;

//陀螺仪DEC2_M2滤波器阶数
typedef enum{
    SDRV_ICM_GYRODEC2M2ORD_3   = 0b10,      //3阶滤波器
}sDRV_ICM_GYRODEC2M2ORD_t;

//加速度计LPF滤波带宽
typedef enum{
    SDRV_ICM_ACCELUIFILTBW_LNODRF2       = 0,   //LN模式,BW=ODR/2
    SDRV_ICM_ACCELUIFILTBW_LNODRF4       = 1,   //BW=max(400Hz, ODR)/4 (默认)
    SDRV_ICM_ACCELUIFILTBW_LNODRF5       = 2,   //BW=max(400Hz, ODR)/5
    SDRV_ICM_ACCELUIFILTBW_LNODRF8       = 3,   //BW=max(400Hz, ODR)/8
    SDRV_ICM_ACCELUIFILTBW_LNODRF10      = 4,   //BW=max(400Hz, ODR)/10
    SDRV_ICM_ACCELUIFILTBW_LNODRF16      = 5,   //BW=max(400Hz, ODR)/16
    SDRV_ICM_ACCELUIFILTBW_LNODRF20      = 6,   //BW=max(400Hz, ODR)/20
    SDRV_ICM_ACCELUIFILTBW_LNODRF40      = 7,   //BW=max(400Hz, ODR)/40
    SDRV_ICM_ACCELUIFILTBW_LNLLODR       = 14,  //低延时模式Dec2 max(400Hz, ODR)
    SDRV_ICM_ACCELUIFILTBW_LNLLODRM8     = 15,  //低延时模式Dec2 max(400Hz, 8*ODR)
    SDRV_ICM_ACCELUIFILTBW_LP1AVG        = 1,   //LP模式 1个样本的平均滤波
    SDRV_ICM_ACCELUIFILTBW_LP16AVG       = 6,   //LP模式 16个样本的平均滤波
}sDRV_ICM_ACCELUIFILTBW_t;

//陀螺仪LPF滤波带宽
typedef enum{
    SDRV_ICM_GYROUIFILTBW_LNODRF2       = 0,   //LN模式,BW=ODR/2
    SDRV_ICM_GYROUIFILTBW_LNODRF4       = 1,   //BW=max(400Hz, ODR)/4 (默认)
    SDRV_ICM_GYROUIFILTBW_LNODRF5       = 2,   //BW=max(400Hz, ODR)/5
    SDRV_ICM_GYROUIFILTBW_LNODRF8       = 3,   //BW=max(400Hz, ODR)/8
    SDRV_ICM_GYROUIFILTBW_LNODRF10      = 4,   //BW=max(400Hz, ODR)/10
    SDRV_ICM_GYROUIFILTBW_LNODRF16      = 5,   //BW=max(400Hz, ODR)/16
    SDRV_ICM_GYROUIFILTBW_LNODRF20      = 6,   //BW=max(400Hz, ODR)/20
    SDRV_ICM_GYROUIFILTBW_LNODRF40      = 7,   //BW=max(400Hz, ODR)/40
    SDRV_ICM_GYROUIFILTBW_LNLLODR       = 14,  //低延时模式Dec2 max(400Hz, ODR)
    SDRV_ICM_GYROUIFILTBW_LNLLODRM8     = 15,  //低延时模式Dec2 max(400Hz, 8*ODR)
}sDRV_ICM_GYROUIFILTBW_t;

//加速度计滤波阶数
typedef enum{
    SDRV_ICM_ACCELUIFILTORD_1   = 0b00,      //1阶滤波器
    SDRV_ICM_ACCELUIFILTORD_2   = 0b01,      //2阶滤波器
    SDRV_ICM_ACCELUIFILTORD_3   = 0b10,      //3阶滤波器
}sDRV_ICM_ACCELUIFILTORD_t;

//加速度计DEC2_M2滤波器阶数
typedef enum{
    SDRV_ICM_ACCELDEC2M2ORD_3   = 0b10,      //3阶滤波器
}sDRV_ICM_ACCELDEC2M2ORD_t;

//选择寄存器bank
typedef enum{
    SDRV_ICM_REGBANKSEL_BANK0   = 0b000,     //默认
    SDRV_ICM_REGBANKSEL_BANK1   = 0b001,
    SDRV_ICM_REGBANKSEL_BANK2   = 0b010,
    SDRV_ICM_REGBANKSEL_BANK3   = 0b011,
    SDRV_ICM_REGBANKSEL_BANK4   = 0b100,
}sDRV_ICM_REGBANKSEL_t;


//配置
typedef struct{
    sDRV_ICM_GYROFS_t         gyro_fs;              //陀螺仪量程
    sDRV_ICM_GYROODR_t        gyro_odr;             //陀螺仪输出速率
    sDRV_ICM_ACCELFS_t        accel_fs;             //加速度计量程
    sDRV_ICM_ACCELODR_t       accel_odr;            //加速度计输出速率
    sDRV_ICM_TEMPFILTBW_t     temp_filt_bw;         //温度DLPF带宽
    sDRV_ICM_GYROUIFILTORD_t  gyro_ui_filt_ord;     //陀螺仪DLPF阶数
    sDRV_ICM_GYRODEC2M2ORD_t  gyro_dec2_m2_ord;     //陀螺仪DEC2_M2滤波器阶数
    sDRV_ICM_ACCELUIFILTBW_t  accel_ui_filt_bw;     //加速度计滤波器带宽
    sDRV_ICM_GYROUIFILTBW_t   gyro_ui_filt_bw;      //陀螺仪滤波器带宽
    sDRV_ICM_ACCELUIFILTORD_t accel_ui_filt_ord;    //陀螺仪DLPF阶数
    sDRV_ICM_ACCELDEC2M2ORD_t accel_dec2_m2_ord;    //陀螺仪DEC2_M2滤波器阶数
    //sDRV_ICM_REGBANKSEL_t     reg_bank_sel;         //寄存器bank配置
}sDRV_ICM_Conf_t;

extern sDRV_ICM_Data_t g_icm;
extern sDRV_ICM_Conf_t g_icm_conf;


int sDRV_ICM_Init();

void sDRV_ICM_SetConfig(const sDRV_ICM_Conf_t* p_conf);

void sDRV_ICM_GetData();

float sDRV_ICM_GetTemp();

#ifdef __cplusplus
}
#endif


