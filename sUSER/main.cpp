#include "main.h"



//128+64K SRAM
//1024K FLASH
//168MHz 210DMIPS

//compile env:
//AC6 C++14




sDBG_UART dbg;

int main(){
    HAL_Init();
    sBSP_RCC_Init();

    HAL_InitTick(0);

    dbg.init(USART1);
    
    uint32_t sys_clk_freq = HAL_RCC_GetSysClockFreq();

    sAPP_Btns_Init();

    dbg.printf("STM32 System Clock Freq: %u MHz\n", sys_clk_freq / 1000000);
    dbg.printf("Hello,STM32F405RGT6    BySightseer.\n");
    


    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC,&gpio);

    __GPIOC_CLK_ENABLE();
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOC,&gpio);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
    


    /*SPI2 <-> IMU_SPI*/
    sBSP_SPI_IMU_Init(SPI_BAUDRATEPRESCALER_64);   //11.25MBits/s

    #define ICM_CS_Pin GPIO_PIN_0
    #define ICM_CS_GPIO_Port GPIOC
    #define LIS3_CS_Pin GPIO_PIN_1
    #define LIS3_CS_GPIO_Port GPIOC

    __GPIOA_CLK_ENABLE();
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA,&gpio);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);

    /*把IMU的2个CS都上拉*/
    __GPIOC_CLK_ENABLE();
    //GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = ICM_CS_Pin | LIS3_CS_Pin;
    HAL_GPIO_Init(GPIOC,&gpio);
    HAL_GPIO_WritePin(GPIOC,ICM_CS_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC,LIS3_CS_Pin,GPIO_PIN_SET);

    __GPIOA_CLK_ENABLE();
    //GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA,&gpio);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);


    __GPIOC_CLK_ENABLE();
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOC,&gpio);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);


    
    sDRV_PL_Init();
    sDRV_PL_SetBrightness(50);


//     sBSP_TIM_Motor_Init();
//     /*actual freq:21KHz*/
//     sBSP_TIM_Motor_SetPWMFreq(20000);


//     sBSP_TIM_Motor_L1SetEN(1);
//     sBSP_TIM_Motor_L2SetEN(1);
//     sBSP_TIM_Motor_R1SetEN(1);
//     sBSP_TIM_Motor_R2SetEN(1);


//    sBSP_TIM_Motor_L1SetDuty(50);
//    sBSP_TIM_Motor_R1SetDuty(50);


    sDRV_DRV8870_Init();
    sDRV_DRV8870_SetLeftPct(100);

    sDRV_DRV8870_SetRightPct(100);

    sDRV_GMR_Init();
    


    // /*初始化传感器*/
    // if(sDRV_ICM_Init() != 0){
    //     dbg.printf("IMU init failed.\n");
    //     while(1);
    // }
    
    // if(sDRV_LIS3_Init() != 0){
    //     dbg.printf("MAG init failed.\n");
    //     while(1);
    // }

    

    

    while(1){
        //sDRV_GMR_Handler();

        //dbg.printf("%6.2f,%6.2f\n",sDRV_GMR_GetLeftRPM(),sDRV_GMR_GetRightRPM());


        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);

        for(int i = 0;i < 100;i++){
            sDRV_PL_SetBrightness(i);
            HAL_Delay(20);
        }
        for(int i = 100;i != 0;i--){
            sDRV_PL_SetBrightness(i);
            HAL_Delay(20);
        }
        
        //HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_4);

        //sDRV_ICM_GetData();
        //sDRV_LIS3_GetData();

        // dbg.printf("%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f\n",g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_icm.temp);

        // dbg.printf("%6.2f,%6.2f,%6.2f,%6.2f\n",g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z,\
        // g_lis3.temp);

        HAL_Delay(100);

        

        //sGBD_Handler();

        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
        // HAL_Delay(1000);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
        // HAL_Delay(1000);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
        // HAL_Delay(1000);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
        // HAL_Delay(1000);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
        // HAL_Delay(1000);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
        // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
        // HAL_Delay(1000);


    }
}

/**
 * 验证项目
 * 
 * LED 有源蜂鸣器 最小系统 调试串口 验证完成
 * 
 * 241009 PM05:14
 * 4x按键+sGBD2验证完成
 * 
 * 241009 PM05:30
 * ICM42688+LIS3 9轴惯导验证完成
 * 
 * 
 * 241010 PM08:22
 * 电机驱动 验证完成
 * 
 * 241011 PM7:05
 * GMR编码器 验证完成
 * 
 * 
 * Track寻迹管
 * 
 * 
 * 灯光控制
 * 
 * 
 * FeRAM 
 * 
 * 
 * OLED
 * 
 * 
 * INA219 电池ADC
 * 
 * 
 * topUART I2C2
 * 
 * 
 * FreeRTOS移植
 * 
 * 
 * cmBacktrace移植完成
 * 
 * 
 * 上层代码
 * 
 * 
 * PMDC C++
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

/**
 * 笔记
 * 
 * 241011 PM
 * 如果发现C++语法报错,检查一下是否在.c/.h(有extern"C")的文件里引用了C++头文件,比如main.h
 * 
 * 
 * 
 * 
 */


