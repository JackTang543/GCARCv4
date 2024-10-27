#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"
#include "sDBG_Debug.h"





typedef void(*sBSP_UART_RecvEndCb_t)(char* pReciData,uint16_t length);




int sBSP_UART_Debug_Init(uint32_t bandrate);
void sBSP_UART_Debug_RecvBegin(sBSP_UART_RecvEndCb_t recv_cb);
void sBSP_UART_Debug_Printf(const char *fmt,...);

int sBSP_UART_Top_Init(uint32_t bandrate);
void sBSP_UART_Top_Printf(const char *fmt,...);
void sBSP_UART_Top_RecvBegin(sBSP_UART_RecvEndCb_t recv_cb);



#ifdef __cplusplus
}
#endif

