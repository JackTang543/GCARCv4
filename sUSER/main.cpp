#include "main.h"



//128+64K SRAM
//1024K FLASH
//168MHz 210DMIPS

//compile env:
//AC6 C++14


/**
 * 完成TOP UART串口驱动 DMA收发
 * 已完成:串口1 DMA+空闲中断接收 241031
 * 
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


sG2D oled;

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


    // sG2D_Init();

    // sG2D_Printf(10,10,"Hello sGCARCv4");

    // sG2D_UpdateScreen();



    sDRV_GenOLED_Init();

    oled.init();
    oled.setFPSMode(sG2D::DIGITS2);

    sAPP_BlcCtrl_Init();

    ahrs.init();
    //ahrs.calcBias();


    // sAPP_Tasks_CreateAll();
    // sBSP_UART_Debug_Printf("FreeRTOS启动任务调度\n");
    // vTaskStartScheduler();    
    int i = 0;



    while(1){
        oled.printf(10,50,"%u",i);
        i++;
        sBSP_DWT_MeasureStart();
        oled.handler();
        oled.setAll(0);
        sBSP_DWT_MeasureEnd();
        sBSP_UART_Debug_Printf("%uus\n",sBSP_DWT_GetMeasure_us());


        //bReadPS2Data();


        // dbg.printf("%u,%.4f,%.4f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);

        // dbg.printf("%6u,%8.4f,%8.4f,%8.4f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        // g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);

        //dbg.printf("%6u,%8.1f,%8.1f,%8.1f,%6.1f,%6.1f,%6.1f,%6.1f,%6.1f,%6.1f\n",HAL_GetTick(),g_icm.acc_x,g_icm.acc_y,g_icm.acc_z,\
        g_icm.gyro_x,g_icm.gyro_y,g_icm.gyro_z,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z);


        HAL_Delay(10);

        

    }
}

    /**
     * 2024.11.01
     * 没有任何优化:需要10090us=10.1ms SPI带宽21MBit/s
     * 使用DMA优化清屏:需要6.9ms
     * 修改GRAM组织结构,从二维数组变成一维,使用SPI以128bytes为一组,一次发送128字节,发8次:1240us=1.24ms
     * 此时完成清空缓冲区GRAM+刷屏的时间只需要1.24ms了,已经非常不错了,不过现在发送还没有用上DMA呢
     * 
     * 修改OLED驱动:初始化配置参数:页地址自增模式,这样就不用发8次了,一次就能发1024个字节,刷完整块屏幕
     * 用时:1075us=1.1ms,这次提升不大~就是少了个for8次的循环
     * 
     * 我们现在使用异步模式,修改SPI的BSP,使用DMA发送1K的显存
     * DMA配置:Normal,数据对齐byte,启动fifo,阈值full,内存突发16,外设突发single,这样配置可以降低总线负荷
     * 用时(非阻塞式):1022us=1.02ms,虽然没多少的时间优化,但是1ms内是非阻塞的,CPU可以干其他活
     * 此时,OLED刷屏需要的CPU时间几乎为0,不消耗CPU性能,但是需要总线带宽,但是就算是60FPS,速率也只有60KB/s的带宽
     * 
     * 此时,这个刷屏还是我们手动启动的,我们也可以使用Circular模式,就不需要管了,全自动刷新屏幕,但是这样是会有问题的
     * 我往GRAM里写的时候DMA有可能在读,此时就可能数据出错,所以我们还是手动启动刷屏,但是也要注意,
     * 要把刷屏和绘图工作放在一个任务里,绘制完就启动刷屏,释放CPU,否则就要加lock
     * 
     * 按道理来说,21MBit/s的SPI每秒可以有2.6MB的速率,但是我们这里才1000*1KB=1MB/s的速度,不对劲~~~
     * 
     * 之前都是O0优化,我们把优化开到O3试试
     * 用时:519us=0.52ms,好的这次对劲了,原来是我们的程序层层封装,消耗了大量性能
     * 1923*1KB=1923KB/s=1.9MB/s,已经很接近2.6MB的最大速度了,只差0.7MB/s了
     * 这0.7MB/s的性能损失很可能是函数调用和HAL库的损失,如果改用寄存器操作,速度还能更快
     * 
     * 我们加入双缓冲区机制
     * 让OLED在刷屏的时候同时进行绘制
     * 
     */



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


