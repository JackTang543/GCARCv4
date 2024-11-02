
#include "stm32f4xx_it.h"

extern UART_HandleTypeDef uart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern UART_HandleTypeDef uart6;

extern TIM_HandleTypeDef htim6;


extern DMA_HandleTypeDef hdma_spi1_tx;



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
    HAL_UART_IRQHandler(&uart1);
}

void USART6_IRQHandler(void){
    HAL_UART_IRQHandler(&uart6);
}



void DMA2_Stream2_IRQHandler(void){
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void DMA2_Stream5_IRQHandler(void){
    HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

void DMA2_Stream7_IRQHandler(void){
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}


void TIM6_DAC_IRQHandler(void){
  uwTick++;
  HAL_TIM_IRQHandler(&htim6);
}


