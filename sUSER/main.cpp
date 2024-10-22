#include "main.h"



//128+64K SRAM
//1024K FLASH
//168MHz 210DMIPS

//compile env:
//AC6 C++14



UART_HandleTypeDef *g_uart1;


void* operator new(std::size_t size) {
    return pvPortMalloc(size);  // FreeRTOS memory allocation
}

void operator delete(void* ptr) noexcept {
    vPortFree(ptr);  // FreeRTOS memory free
}

void* operator new[](std::size_t size) {
    return pvPortMalloc(size);  // For array allocation
}

void operator delete[](void* ptr) noexcept {
    vPortFree(ptr);  // For array deallocation
}






int main(){
    car.initSys();
    

    dbg.printf("STM32 System Clock Freq: %u MHz\n", car.coreClk / 1000000);
    dbg.println("Hello,STM32F405RGT6    BySightseer.");



    dbg.println("sGCARCv4初始化完成");

    HAL_Delay(100);

    sBSP_DWT_Init(car.coreClk);


    __GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin   = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC,&gpio);
    

    //初始化IMU
    if(imu.init() != 0){
        dbg.println("IMU 初始化失败");
        Error_Handler();
    }

    
    sDRV_PL_Init();
    sDRV_PL_SetBrightness(50);


    sDRV_GenOLED_Init();
    int i = 0;
    sG2D_Printf(10,10,"Hello sGCARCv4");

    sG2D_UpdateScreen();

    sBSP_ADC_Init();

    //dbg.printf("FreeRTOS启动任务调度\n");
    //vTaskStartScheduler();



    #define NUM 1000
    float gyro_x_accu = 0.0f;
    float gyro_y_accu = 0.0f;
    float gyro_z_accu = 0.0f;
    float acc_x_accu = 0.0f;
    float acc_y_accu = 0.0f;
    float acc_z_accu = 0.0f;
    for(uint32_t i = 0;i < NUM;i++){
        sDRV_ICM_GetData();
        gyro_x_accu += g_icm.gyro_x;
        gyro_y_accu += g_icm.gyro_y;
        gyro_z_accu += g_icm.gyro_z;
        acc_x_accu += g_icm.acc_x;
        acc_y_accu += g_icm.acc_y;
        acc_z_accu += g_icm.acc_z;
        HAL_Delay(1);
    }

    float gyro_x_bias = gyro_x_accu / 1000.0f;
    float gyro_y_bias = gyro_y_accu / 1000.0f;
    float gyro_z_bias = gyro_z_accu / 1000.0f;

    float acc_x_bias = acc_x_accu / 1000.0f;
    float acc_y_bias = acc_y_accu / 1000.0f;
    float acc_z_bias = acc_z_accu / 1000.0f - 9.81f;


    

    //todo 完成ADC OLED库 动画库 

    while(1){

        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);


        sDRV_ICM_GetData();
        sDRV_LIS3_GetData();

        g_icm.gyro_x -= gyro_x_bias;
        g_icm.gyro_y -= gyro_y_bias;
        g_icm.gyro_z -= gyro_z_bias;

        g_icm.acc_x -= acc_x_bias;
        g_icm.acc_y -= acc_y_bias;
        g_icm.acc_z -= acc_z_bias;



        // dbg.printf("%u,%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);

        // dbg.printf("%6u,%8.4f,%8.4f,%8.4f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);

        dbg.printf("%6u,%8.1f,%8.1f,%8.1f,%6.1f,%6.1f,%6.1f,%6.1f,%6.1f,%6.1f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);


        HAL_Delay(10);

        

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


