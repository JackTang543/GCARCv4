#include "sBSP_DMA.h"


#include "sDBG_Debug.h"

/**
 * DMA2 Stream0:用于把一块内存置任意数
 * 用于STM32F405RGT6
 * v1.1 241026
 * 
 */



//按byte设置某个一块内存值
DMA_HandleTypeDef hdma2_stream0;
#define MEMSETB_HANDLE hdma2_stream0
uint8_t dma2_s0_src_data;

DMA_HandleTypeDef hdma2_stream1;


//memset by byte
int sBSP_DMA_MemSetByte_Init(){
    __HAL_RCC_DMA2_CLK_ENABLE();

    MEMSETB_HANDLE.Instance                 = DMA2_Stream0;
    MEMSETB_HANDLE.Init.Channel             = DMA_CHANNEL_0;
    MEMSETB_HANDLE.Init.Direction           = DMA_MEMORY_TO_MEMORY;
    MEMSETB_HANDLE.Init.PeriphInc           = DMA_PINC_DISABLE;
    MEMSETB_HANDLE.Init.MemInc              = DMA_MINC_ENABLE;
    MEMSETB_HANDLE.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    MEMSETB_HANDLE.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    MEMSETB_HANDLE.Init.Mode                = DMA_NORMAL;
    MEMSETB_HANDLE.Init.Priority            = DMA_PRIORITY_LOW;
    MEMSETB_HANDLE.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    MEMSETB_HANDLE.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    MEMSETB_HANDLE.Init.MemBurst            = DMA_MBURST_SINGLE;
    MEMSETB_HANDLE.Init.PeriphBurst         = DMA_PBURST_SINGLE;

    if(HAL_DMA_Init(&MEMSETB_HANDLE) != HAL_OK){
        assert_param(1);    //直接报错
        return -1;
    }
    return 0;
}

//memset by byte 输入长度在1~65535之间 实测8192个数据(8KB)需要256us完成
int sBSP_DMA_MemSetByte(uint8_t value,uint8_t* pDst,uint16_t len_bytes){
    dma2_s0_src_data = value;
    assert_param(pDst != NULL);
    assert_param(len_bytes > 0 && len_bytes <= 65535);

    if(HAL_DMA_Start(&MEMSETB_HANDLE,(uint32_t)&dma2_s0_src_data,(uint32_t)pDst,len_bytes) != HAL_OK){
        sDBG_Debug_Warning("HAL_DMA_Start 错误");
        return -1;
    }
    if(HAL_DMA_PollForTransfer(&MEMSETB_HANDLE, HAL_DMA_FULL_TRANSFER, 1000) != HAL_OK){
        sDBG_Debug_Warning("HAL_DMA_PollForTransfer 错误");
        return -1;
    }
    return 0;
}

void sBSP_DMA2S1_Init(void){
    __HAL_RCC_DMA2_CLK_ENABLE(); // 启用DMA时钟

    
    hdma2_stream1.Instance = DMA2_Stream1;
    hdma2_stream1.Init.Channel = DMA_CHANNEL_2;
    hdma2_stream1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma2_stream1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma2_stream1.Init.MemInc = DMA_MINC_ENABLE;
    hdma2_stream1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma2_stream1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma2_stream1.Init.Mode = DMA_NORMAL;
    hdma2_stream1.Init.Priority = DMA_PRIORITY_LOW;
    hdma2_stream1.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma2_stream1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma2_stream1.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma2_stream1.Init.PeriphBurst = DMA_PBURST_SINGLE;
    
    // 将DMA与SPI关联
    __HAL_LINKDMA(&hspi1, hdmatx, hdma2_stream1);
    
    // 初始化DMA
    HAL_DMA_Init(&hdma2_stream1);
    
    // 启用DMA传输完成中断
    //HAL_NVIC_SetPriority(DMAx_STREAMx_IRQn, 0, 1);
    //HAL_NVIC_EnableIRQ(DMAx_STREAMx_IRQn);
}

void sBSP_DMA2S1_32MemToSPI1(uint32_t* pSrc,uint32_t len_bytes){
    HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*)pSrc,len_bytes / 4);
    //HAL_Delay(10);
    //vTaskDelay(10);
    //HAL_DMA_Start(&hdma2_stream1,(uint32_t)pSrc,(uint32_t)&hspi1.Instance->DR,len_bytes / 4);
    //HAL_DMA_PollForTransfer(&hdma2_stream1, HAL_DMA_FULL_TRANSFER, 100);
}




