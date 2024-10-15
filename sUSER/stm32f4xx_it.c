
#include "stm32f4xx_it.h"

//extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef htim6;



void NMI_Handler(void){
  while (1){
      
  }
}

// void HardFault_Handler(void){
//   while (1){
      
//   }
// }

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

// void SysTick_Handler(void){
//     HAL_IncTick();
// }



void DebugMon_Handler(void){
    
}


void USART1_IRQHandler(void){
    //HAL_UART_IRQHandler(&huart1);
}


void TIM6_DAC_IRQHandler(void){
  uwTick++;
  HAL_TIM_IRQHandler(&htim6);
}


