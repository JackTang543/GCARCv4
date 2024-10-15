#include "sDBG_Debug.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char fmt_buf[256];

void sDBG_Debug_Printf(const char *fmt,...){
    va_list ap;
    va_start(ap,fmt);
    vsprintf(fmt_buf,fmt,ap);
    va_end(ap);

    extern UART_HandleTypeDef *g_uart1;
    HAL_UART_Transmit(g_uart1, (uint8_t*)fmt_buf, (uint16_t)(strlen(fmt_buf)), 100);
}

void Error_Handler(){
    __disable_irq();
    while (1){
        sDBG_Debug_Printf("WARNING警告:错误! 禁用IRQ,死循环...\n");
        HAL_Delay(500);
    }
}




void assert_failed(uint8_t* file, uint32_t line){
    __disable_irq();
    while (1){
        sDBG_Debug_Printf("WARNING警告:断言! 文件名:%s,行:%u 禁用IRQ,死循环...\n",file,line);
        HAL_Delay(500);
    }
}

void vApplicationMallocFailedHook(){
    sDBG_Debug_Printf("WARNING警告:内存申请失败! 禁用IRQ\n");
}

void vApplicationIdleHook(){
    
}   

void vApplicationTickHook(){
}


void vApplicationStackOverflowHook(TaskHandle_t xTask,char* pcTaskName){
    sDBG_Debug_Printf("WARNING警告:触发栈金丝雀机制! 任务句柄:0x%X,任务名:%s\n",xTask,pcTaskName);
}


