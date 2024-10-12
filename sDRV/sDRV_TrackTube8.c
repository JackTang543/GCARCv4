#include "sDRV_TrackTube8.h"

//赵老师的基于74hc165的8位寻迹模块
//240715
//241012 v1.1 软件CS在本文件里定义初始化

//! sGCARCv4 好像接口数据引脚不太对，反了

//从正面看,从左到右为MSB->LSB

//PD2 -> CS
#define SPI_CS_CLK_EN    __GPIOD_CLK_ENABLE
#define SPI_CS_PORT      GPIOD
#define SPI_CS_PIN       GPIO_PIN_2


/***************************************对底层接口*************************************/
static inline uint8_t portGetByte(void){
    return sBSP_SPI_TRACK_RecvByte();
}

static void portCSInit(){
    SPI_CS_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = SPI_CS_PIN;
    HAL_GPIO_Init(SPI_CS_PORT,&gpio);
}

static inline void portSetCS(bool cs_lv){
    HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,(GPIO_PinState)cs_lv);
}

//float格式的映射,类似Arduino的map
static inline float fmap(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void sDRV_TrackTube8_Init(){
    //初始化SPI,这里必须用软件NSS,和标准SPI的NSS不一样,读之前给一个正脉冲,读完也给一个
    sBSP_SPI_TRACK_Init(SPI_BAUDRATEPRESCALER_256);
    portCSInit();
    sBSP_SPI_TRACK_SetEN(1);

    //跳过0xFF
    float dummy = 0;
    sDRV_TrackTube8_GetData(&dummy,&dummy);
}


//结果位置,-1~1
float result_pos;
float prev_pos;
//光电管原始数据
uint8_t raw_tube;

//获取数据,返回值:检测的状态,如果检测到两条路径,则两个形参都有效,否则只有形参1有效
sDRV_TrackTube8_Status_t sDRV_TrackTube8_GetData(float* pPath1,float* pPath2){
    //给一个正脉冲
    portSetCS(0);
    portSetCS(1);
    raw_tube = ~portGetByte();
    //再给一个
    portSetCS(0);
    portSetCS(1);
    //数据读到了,现在开始处理


    prev_pos = result_pos;

    //首先检查光电管是否全亮
    if(raw_tube == 0xFF){
        return SDRV_TRACKTUBE8_ALLON;
    }


    for(uint8_t i = 0; i < 8; i++){
        //检查线的位置
        if(raw_tube & (1 << i)){
            if((raw_tube & (1 << (i + 1))) && (i != 7)){
                //点亮了两个光电管
                result_pos = i + 0.5;
                
            }else{
                //只点亮了一个光电管
                result_pos = i;
            }
            //数据归一化
            result_pos = fmap(result_pos, 0, 7, -1, 1);
            *pPath1 = result_pos;
            return SDRV_TRACKTUBE8_VALID;
        }
    }

    if(raw_tube == 0){
        *pPath1 = prev_pos;
    }


    return SDRV_TRACKTUBE8_VALID;
}




