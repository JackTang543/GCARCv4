#include "sBSP_UART.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>


/*Debug串口 UART1*/
UART_HandleTypeDef uart1;
uint32_t uart1_blocking_ms = 100;
//printf格式化
char uart1_fmt_buf[256];
//串口接收缓冲
char uart1_recv_buf[256];
//保存用户传入的接收完成回调
sBSP_UART_RecvEndCb_t uart1_recv_end_cb;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/*TOP串口 UART6*/
UART_HandleTypeDef uart6;
uint32_t uart6_blocking_ms = 100;
//printf格式化
char uart6_fmt_buf[256];
//串口接收缓冲
char uart6_recv_buf[256];
//保存用户传入的接收完成回调
sBSP_UART_RecvEndCb_t uart6_recv_end_cb;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;



int sBSP_UART_Debug_Init(uint32_t bandrate){
    uart1.Instance          = USART1;
    uart1.Init.BaudRate     = bandrate;
    uart1.Init.WordLength   = UART_WORDLENGTH_8B;
    uart1.Init.StopBits     = UART_STOPBITS_1;
    uart1.Init.Parity       = UART_PARITY_NONE;
    uart1.Init.Mode         = UART_MODE_TX_RX;
    uart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    uart1.Init.OverSampling = UART_OVERSAMPLING_16;


    if (HAL_UART_Init(&uart1) != HAL_OK){
        return -1;
    }
    return 0;
}


void sBSP_UART_Debug_Printf(const char *fmt,...){
    va_list ap;
    va_start(ap,fmt);
    vsprintf(uart1_fmt_buf,fmt,ap);
    va_end(ap);

    HAL_UART_Transmit(&uart1, (uint8_t*)uart1_fmt_buf, (uint16_t)(strlen(uart1_fmt_buf)), uart1_blocking_ms);
}




void sBSP_UART_Debug_SendByte(uint8_t byte){

}


void sBSP_UART_Debug_SendBytes(uint8_t* pData,uint16_t length){

}


void sBSP_UART_Debug_RecvBegin(sBSP_UART_RecvEndCb_t recv_cb){
    assert_param(recv_cb != NULL);
    uart1_recv_end_cb = recv_cb;

    // if(HAL_UARTEx_ReceiveToIdle_IT(&uart1, (uint8_t*)uart1_recv_buf, sizeof(uart1_recv_buf)) != HAL_OK){
    //     sDBG_Debug_Warning("串口1:空闲中断IT接收出错");
    // }

    if(HAL_UARTEx_ReceiveToIdle_DMA(&uart1, (uint8_t*)uart1_recv_buf, sizeof(uart1_recv_buf)) != HAL_OK){
        sDBG_Debug_Warning("串口1:空闲中断DMA接收出错");
    }
}


int sBSP_UART_Top_Init(uint32_t bandrate){
    uart6.Instance          = USART6;
    uart6.Init.BaudRate     = bandrate;
    uart6.Init.WordLength   = UART_WORDLENGTH_8B;
    uart6.Init.StopBits     = UART_STOPBITS_1;
    uart6.Init.Parity       = UART_PARITY_NONE;
    uart6.Init.Mode         = UART_MODE_TX_RX;
    uart6.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    uart6.Init.OverSampling = UART_OVERSAMPLING_16;


    if (HAL_UART_Init(&uart6) != HAL_OK){
        return -1;
    }
    return 0;
}

void sBSP_UART_Top_Printf(const char *fmt,...){
    va_list ap;
    va_start(ap,fmt);
    vsprintf(uart6_fmt_buf,fmt,ap);
    va_end(ap);

    HAL_UART_Transmit(&uart6, (uint8_t*)uart6_fmt_buf, (uint16_t)(strlen(uart6_fmt_buf)), uart6_blocking_ms);
}

void sBSP_UART_Top_RecvBegin(sBSP_UART_RecvEndCb_t recv_cb){
    assert_param(recv_cb != NULL);
    uart6_recv_end_cb = recv_cb;

    if(HAL_UARTEx_ReceiveToIdle_IT(&uart6, (uint8_t*)uart6_recv_buf, sizeof(uart6_recv_buf)) != HAL_OK){
        sDBG_Debug_Warning("串口6:空闲中断IT接收出错");
    }

    // if(HAL_UARTEx_ReceiveToIdle_DMA(&uart6, (uint8_t*)uart6_recv_buf, sizeof(uart6_recv_buf)) != HAL_OK){
    //     sDBG_Debug_Warning("串口6:空闲中断DMA接收出错");
    // }
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if (huart->Instance == USART1){
        assert_param(uart1_recv_end_cb != NULL);
        uart1_recv_end_cb(uart1_recv_buf,Size);
    }
    else if(huart->Instance == USART6){
        assert_param(uart6_recv_end_cb != NULL);
        uart6_recv_end_cb(uart6_recv_buf,Size);
    }
}






