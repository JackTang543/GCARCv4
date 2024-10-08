
#include "stm32f4xx_it.h"

//extern UART_HandleTypeDef huart1;


void NMI_Handler(void){
  while (1){
      
  }
}

void HardFault_Handler(void){
  while (1){
      
  }
}

void MemManage_Handler(void){
  while (1){
      
  }
}

void BusFault_Handler(void){
  while (1){
      
  }
}

void UsageFault_Handler(void){
  while (1){
      
  }
}

void SysTick_Handler(void){
    HAL_IncTick();
}



void DebugMon_Handler(void){
    
}


void USART1_IRQHandler(void){
    //HAL_UART_IRQHandler(&huart1);
}

