#pragma once



#include "stm32f4xx_hal.h"

#include <cstdio>
#include <string.h>
#include <stdarg.h>


enum class sDBG_UART_MODE{
    BLOCKING = 0,
    INTERRUPT,
    DMA,
};

class sDBG_UART{

public:
    sDBG_UART();
    ~sDBG_UART();

    UART_HandleTypeDef uart_handle;


    void init(USART_TypeDef* uart_ins);
    void init(USART_TypeDef* uart_ins,uint32_t bandrate);

    void init_all(USART_TypeDef* ins,uint32_t bandrate,uint32_t word_len,uint32_t stop_bits,\
            uint32_t parity,uint32_t mode,uint32_t hw_flow_ctl,uint32_t oversampling);

    void setSendMode(sDBG_UART_MODE send_mode);


    void printf(const char *fmt,...);

    void print();
    void print(int number);
    void print(unsigned int number);
    void print(const char* str);

    void println();
    void println(int number);
    void println(unsigned int number);
    void println(const char* str);

private:

    sDBG_UART_MODE send_mode = sDBG_UART_MODE::BLOCKING;
    sDBG_UART_MODE recv_mode = sDBG_UART_MODE::BLOCKING;

    //all instances use this variable
    static uint32_t blocking_max_time;

    //used for printf function
    char fmt_buf[128];







};





