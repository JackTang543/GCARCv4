#include "sAPP_Car.hpp"

#include "main.h"

extern UART_HandleTypeDef *g_uart1;
sAPP_Car car;




sAPP_Car::sAPP_Car(){

}

sAPP_Car::~sAPP_Car(){

}




int sAPP_Car::initSys(){
    //初始化系统
    sBSP_SYS_Init();
    //初始化调试串口
    dbg.init(USART1);
    g_uart1 = &dbg.uart_handle;
    //初始化IMU
    imu.init();
    //初始化电机
    motor.init();

    //获取时钟频率
    coreClk = HAL_RCC_GetSysClockFreq();

    //初始化按键
    sAPP_Btns_Init();

    sBSP_I2C1_Init(400000);
    
    

    //启用div0异常
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;

    dbg.printf("cm_backtrace初始化完成\n");
    cm_backtrace_init(APPNAME, HARDWARE_VERSION, SOFTWARE_VERSION);

    return 0;
}




