#include "sDBG_UART.hpp"

#include "sDBG_Debug.h"


uint32_t sDBG_UART::blocking_max_time = 50;


sDBG_UART::sDBG_UART(){

}


sDBG_UART::~sDBG_UART(){

}


//init the uart.this overloading function1 accepts a instance,default params:115200,8N1
void sDBG_UART::init(USART_TypeDef* uart_ins){
    init_all(
    uart_ins, \
    115200, \
    UART_WORDLENGTH_8B, \
    UART_STOPBITS_1, \
    UART_PARITY_NONE,\
    UART_MODE_TX_RX, \
    UART_HWCONTROL_NONE, \
    UART_OVERSAMPLING_16 \
    );

    //init the uart
    if (HAL_UART_Init(&this->uart_handle) != HAL_OK){
        Error_Handler();
    }
}


//init the uart.this overloading function2 accepts a instance and bandrate,default params:8N1
void sDBG_UART::init(USART_TypeDef* uart_ins,uint32_t bandrate){
    init_all(
    uart_ins, \
    bandrate, \
    UART_WORDLENGTH_8B, \
    UART_STOPBITS_1, \
    UART_PARITY_NONE,\
    UART_MODE_TX_RX, \
    UART_HWCONTROL_NONE, \
    UART_OVERSAMPLING_16 \
    );
}

void sDBG_UART::init_all(USART_TypeDef* ins,uint32_t bandrate,uint32_t word_len,uint32_t stop_bits,\
                    uint32_t parity,uint32_t mode,uint32_t hw_flow_ctl,uint32_t oversampling){
    this->uart_handle.Instance = ins;
    this->uart_handle.Init.BaudRate = bandrate;
    this->uart_handle.Init.WordLength = word_len;
    this->uart_handle.Init.StopBits = stop_bits;
    this->uart_handle.Init.Parity = parity;
    this->uart_handle.Init.Mode = mode;
    this->uart_handle.Init.HwFlowCtl = hw_flow_ctl;
    this->uart_handle.Init.OverSampling = oversampling;
    if (HAL_UART_Init(&this->uart_handle) != HAL_OK){
        Error_Handler();
    }
}


void sDBG_UART::setSendMode(sDBG_UART_MODE send_mode){
    this->send_mode = send_mode;
}




//print method overloading. print a number to uart
void sDBG_UART::print(uint32_t number){
    if(this->send_mode == sDBG_UART_MODE::BLOCKING){
        char buf[10];
        snprintf(buf, sizeof(buf), "%d", number);
        HAL_UART_Transmit(&this->uart_handle, (uint8_t*)buf, (uint16_t)(strlen(buf)), blocking_max_time);
    }

}


void sDBG_UART::println(uint32_t number){
    print(number);  print('\n');        //todo 有问题，得有一个专门打印换行符
}



