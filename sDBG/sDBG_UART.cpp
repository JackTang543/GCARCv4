// #include "sDBG_UART.hpp"

// #include "sDBG_Debug.h"


// /**
//  * v1.1 241025 加入注册回调功能,可以接收数据了
//  * 
//  */



// sDBG_UART dbg;

// uint32_t sDBG_UART::blocking_max_time = 50;


// sDBG_UART::sDBG_UART(){

// }


// sDBG_UART::~sDBG_UART(){

// }


// //init the uart.this overloading function1 accepts a instance,default params:115200,8N1
// void sDBG_UART::init(USART_TypeDef* uart_ins){
//     init_all(
//     uart_ins, \
//     115200, \
//     UART_WORDLENGTH_8B, \
//     UART_STOPBITS_1, \
//     UART_PARITY_NONE,\
//     UART_MODE_TX_RX, \
//     UART_HWCONTROL_NONE, \
//     UART_OVERSAMPLING_16 \
//     );

//     //init the uart
//     if (HAL_UART_Init(&this->uart_handle) != HAL_OK){
//         Error_Handler();
//     }

    

// }


// //init the uart.this overloading function2 accepts a instance and bandrate,default params:8N1
// void sDBG_UART::init(USART_TypeDef* uart_ins,uint32_t bandrate){
//     init_all(
//     uart_ins, \
//     bandrate, \
//     UART_WORDLENGTH_8B, \
//     UART_STOPBITS_1, \
//     UART_PARITY_NONE,\
//     UART_MODE_TX_RX, \
//     UART_HWCONTROL_NONE, \
//     UART_OVERSAMPLING_16 \
//     );
// }

// void sDBG_UART::init_all(USART_TypeDef* ins,uint32_t bandrate,uint32_t word_len,uint32_t stop_bits,\
//                     uint32_t parity,uint32_t mode,uint32_t hw_flow_ctl,uint32_t oversampling){
//     this->uart_handle.Instance = ins;
//     this->uart_handle.Init.BaudRate = bandrate;
//     this->uart_handle.Init.WordLength = word_len;
//     this->uart_handle.Init.StopBits = stop_bits;
//     this->uart_handle.Init.Parity = parity;
//     this->uart_handle.Init.Mode = mode;
//     this->uart_handle.Init.HwFlowCtl = hw_flow_ctl;
//     this->uart_handle.Init.OverSampling = oversampling;
//     if (HAL_UART_Init(&this->uart_handle) != HAL_OK){
//         Error_Handler();
//     }
// }


// void sDBG_UART::setSendMode(sDBG_UART_MODE send_mode){
//     this->send_mode = send_mode;
// }

// void sDBG_UART::setRecvMode(sDBG_UART_MODE recv_mode){
//     this->recv_mode = recv_mode;
// }


// //注册回调
// void sDBG_UART::regRecvIdleCb(RecvIdleCb_t cb){
//     this->recv_idle_cb = cb;

// }




// void sDBG_UART::printf(const char *fmt,...){
//     va_list ap;
//     va_start(ap,fmt);
//     vsprintf(fmt_buf,fmt,ap);
//     va_end(ap);

//     if(send_mode == sDBG_UART_MODE::BLOCKING){
//         HAL_UART_Transmit(&uart_handle, (uint8_t*)fmt_buf, (uint16_t)(strlen(fmt_buf)), blocking_max_time);
//     }
// }




// void sDBG_UART::print(){
//     printf(" ");
// }

// //print method overloading. print a number to uart
// void sDBG_UART::print(int number){
//     printf("%d",number);
// }

// void sDBG_UART::print(unsigned int number){
//     printf("%u",number);
// }


// void sDBG_UART::print(const char* str){
//     printf("%s",str);
// }



// void sDBG_UART::println(){
//     printf("\n");
// }

// //print a number to uart,attach up "\n"
// void sDBG_UART::println(int number){
//     printf("%d\n",number);
// }

// void sDBG_UART::println(unsigned int number){
//     printf("%u\n",number);
// }


// void sDBG_UART::println(const char* str){
//     printf("%s\n",str);
// }


// //HAL库的串口接收回调
// extern"C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//     if(huart->Instance == USART1){
//         if(uart1_reci_data_end_cb != NULL){
//             //这里不清空缓冲区,因为有length在
//             uart1_reci_data_end_cb(g_UART1_RxBuf,Size);
//         }
//     }
//     else if(huart->Instance == USART6){
//         if(uart6_reci_data_end_cb != NULL){
//             //这里不清空缓冲区,因为有length在
//             uart6_reci_data_end_cb(UART6_RxBuf,Size);
//         }
//     }
// }






