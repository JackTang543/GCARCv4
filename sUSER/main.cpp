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

    HAL_Delay(100);

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
    


    /*SPI2 <-> IMU_SPI*/
    sBSP_SPI_IMU_Init(SPI_BAUDRATEPRESCALER_64);   //11.25MBits/s

    

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


    
    sDRV_PL_Init();
    sDRV_PL_SetBrightness(50);

    sBSP_I2C1_Init(400000);
    sDRV_INA219_Init();


//     sBSP_TIM_Motor_Init();
//     /*actual freq:21KHz*/
//     sBSP_TIM_Motor_SetPWMFreq(20000);


//     sBSP_TIM_Motor_L1SetEN(1);
//     sBSP_TIM_Motor_L2SetEN(1);
//     sBSP_TIM_Motor_R1SetEN(1);
//     sBSP_TIM_Motor_R2SetEN(1);


//    sBSP_TIM_Motor_L1SetDuty(50);
//    sBSP_TIM_Motor_R1SetDuty(50);


    // sDRV_DRV8870_Init();
    // sDRV_DRV8870_SetLeftPct(100);

    // sDRV_DRV8870_SetRightPct(100);

    // sDRV_GMR_Init();

    // sBSP_I2C1_Init(400000);
    // sDRV_MB85RCxx_Init();
    

    // uint8_t dat[128];

    // sDRV_MB85RCxx_ReadBytes(0x00,dat,128);


    // dbg.printf("Raw:\n");

    // for(int i = 0;i < (128/16);i++){
    //     for(int j = 0;j < 16;j++){
    //     dbg.printf("0x%0X ",dat[i*16+j]);
    //     }
    //     dbg.println();
    // }

    // dbg.printf("Write:\n");

    // //sDRV_MB85RCxx_WriteByte(0x50,0xA1);
    // sDRV_MB85RCxx_Format(0xF2);

    // sDRV_MB85RCxx_ReadBytes(0x00,dat,128);

    // for(int i = 0;i < (128/16);i++){
    //     for(int j = 0;j < 16;j++){
    //     dbg.printf("0x%0X ",dat[i*16 + j]);
    //     }
    //     dbg.println();
    // }

    // /*初始化传感器*/
    // if(sDRV_ICM_Init() != 0){
    //     dbg.printf("IMU init failed.\n");
    //     while(1);
    // }
    
    // if(sDRV_LIS3_Init() != 0){
    //     dbg.printf("MAG init failed.\n");
    //     while(1);
    // }


    // sDRV_TrackTube8_Init();
    // float path1,path2;


    sDRV_GenOLED_Init();
    int i = 0;
    sG2D_Printf(10,10,"Hello sGCARCv4");

    sG2D_UpdateScreen();

    sBSP_ADC_Init();
    
    
    

    while(1){

        float a = sDRV_INA219_GetCurrA();
        float v = sDRV_INA219_GetBusV();
        float w = sDRV_INA219_GetPwrW();

        sG2D_Printf(10,20,"i = %u",i);
        i++;

        sG2D_Printf(10,30,"%.4fA",a);
        sG2D_Printf(10,40,"%.4fV",v);
        sG2D_Printf(10,50,"%.4fW",w);

        sG2D_UpdateScreen();
        sG2D_SetAllGRAM(0);

        float vbat = sBSP_ADC_GetBatVolt();
        dbg.printf("vbat = %.2f\n",vbat);

        //sDRV_GMR_Handler();

        //dbg.printf("%6.2f,%6.2f\n",sDRV_GMR_GetLeftRPM(),sDRV_GMR_GetRightRPM());

        // sDRV_TrackTube8_GetData(&path1,&path2);
        // dbg.printf("%.2f\n",path1);

        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);

        // for(int i = 0;i < 100;i++){
        //     sDRV_PL_SetBrightness(i);
        //     HAL_Delay(20);
        // }
        // for(int i = 100;i != 0;i--){
        //     sDRV_PL_SetBrightness(i);
        //     HAL_Delay(20);
        // }
        
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
 * 241011 PM07:05
 * GMR编码器 验证完成
 * 
 * 241011 PM08:58
 * 灯光控制 验证完成
 * 
 * 241012 AM10:25
 * FeRAM 验证完成
 * 
 * 241012 PM12:01
 * Track寻迹管 //! 好像有问题
 * 
 * 241012 PM02:08
 * OLED 验证完成
 * 
 * 
 * 241012 PM03:43
 * 电池ADC 验证完成
 * 
 * 
 * INA219
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


