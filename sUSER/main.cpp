#include "main.h"



//128+64K SRAM
//1024K FLASH
//168MHz 210DMIPS

//compile env:
//AC6 C++14


/**
 * 完成TOP UART串口驱动 DMA收发
 * 完成PS2驱动
 * 完成sBD
 * 完成sGraphic2D v2 脏区块刷新 简单非线性动画
 * 
 * 电池电量报警
 * 
 * Matlab 磁力计校准算法 自动控制 联调 
 * 
 * FeRAM应用 人
 * 
 * 
 */



#include <stdio.h>

void uart_recied(char* pReciData,uint16_t length){

    if(sscanf(pReciData, "SPD:%f,MOVE:%f,HEAD:%f",\
        &g_ctrl.tar_spd,&g_ctrl.tar_move,&g_ctrl.tar_head) == 3){
        sBSP_UART_Top_Printf("SETTED OK\n");
    }

    //sBSP_UART_Top_Printf("TOP:%s\n",pReciData);

    sBSP_UART_Top_RecvBegin(uart_recied);
}



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

uint8_t IRAM1_ATTR my_buf[8192];


// 遍历内存，检查是否所有字节都为指定值
void check_memory(const void *mem, size_t size, uint8_t target_value) {
    const uint8_t *ptr = (const uint8_t *)mem;  // 将内存转换为字节指针
    for (size_t i = 0; i < size; ++i) {
        if (ptr[i] != target_value) {
            sBSP_UART_Debug_Printf("在索引 %zu 处发现不同的值: 0x%08X\n", i, ptr[i]);
            return;
        }
    }
    sBSP_UART_Debug_Printf("内存中的所有字节都为 0x%08X\n", target_value);
}

int main(){
    car.initSys();
    car.initBoard();

    //sBSP_UART_Debug_RecvBegin(uart_recied);
    sBSP_UART_Top_RecvBegin(uart_recied);

    sBSP_UART_Debug_Printf("STM32 System Clock Freq: %u MHz\n", car.coreClk / 1000000);
    sBSP_UART_Debug_Printf("Hello,STM32F405RGT6    BySightseer.\n");
    sBSP_UART_Debug_Printf("sGCARCv4初始化完成\n");


    sG2D_Printf(10,10,"Hello sGCARCv4");

    sG2D_UpdateScreen();

    sAPP_BlcCtrl_Init();

    ahrs.init();
    //ahrs.calcBias();


    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = TRACK_SCK_Pin|TRACK_MOSI_Pin;
    HAL_GPIO_Init(GPIOC,&gpio);

    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = TRACK_CS_Pin;
    HAL_GPIO_Init(GPIOD,&gpio);

    gpio.Mode  = GPIO_MODE_INPUT;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = TRACK_MISO_Pin;
    HAL_GPIO_Init(TRACK_MISO_GPIO_Port,&gpio);

    HAL_GPIO_WritePin(TRACK_CS_GPIO_Port,TRACK_CS_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC,TRACK_SCK_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,TRACK_MOSI_Pin,GPIO_PIN_SET);


    PS2_SetInit();

    // sBSP_DMA_32MemSet_Init();
    // HAL_Delay(1);

    // #define SET_VAL 0x52
    // #define SET_NUM 8192

    // uint8_t* ptr = &my_buf[0];
    // sBSP_UART_Debug_Printf("0x%X\n",ptr);

    // sBSP_DWT_MeasureStart();
    // //NUM=8192 0x12345678 219us
    // //NUM=8192 0x52 256us
    // sBSP_DMA_32MemSet(SET_VAL,(uint32_t*)my_buf,SET_NUM);
    // sBSP_DWT_MeasureEnd();

    // check_memory(my_buf,SET_NUM,SET_VAL);

    // sBSP_UART_Debug_Printf("%uus\n",sBSP_DWT_GetMeasure_us());



    // sDRV_PS2_Init();

    // motor.setLM(-30);
    // motor.setRM(-30);


    // sAPP_Tasks_CreateAll();
    // sBSP_UART_Debug_Printf("FreeRTOS启动任务调度\n");
    // vTaskStartScheduler();    

    while(1){
        //sBSP_UART_Top_Printf("Hello,CC2530!\n");
        //sBSP_UART_Debug_Printf("Hello,STM32!\n");

        bReadPS2Data();


        // dbg.printf("%u,%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);

        // dbg.printf("%6u,%8.4f,%8.4f,%8.4f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);

        //dbg.printf("%6u,%8.1f,%8.1f,%8.1f,%6.1f,%6.1f,%6.1f,%6.1f,%6.1f,%6.1f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);


        HAL_Delay(100);

        

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


