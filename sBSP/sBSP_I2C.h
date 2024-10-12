#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"

/**
  * sBSP_F4_I2C.h
  * 用于STM32F4平台的I2C接口板级支持包
  * 验证平台:STM32F411CEU6
  * Sightseer's BSP I2C interface
  * 
  * 更新日志:
  * 
  * v1.0  2024.04.16inHNIP9607
  * 初版,F103上验证
  * 
  * v1.1  2024.04.19inHNIP9607
  * 增加了Mem操作,所有操作改成内联函数
  * 
  * v1.2  2024.06.16inHNIP9607
  * 修复I2C中断式传输的bug,移植到F411平台,优化性能,删除是否检查错误宏
  * 
  * v1.3 2024.10.12inHNIP9607
  * 应用到F405RGT6 sGCARCv4平台
  * 
  */



/** @defgroup I2C_XferOptions_definition I2C XferOptions definition
  * @{
  */
// #define  I2C_FIRST_FRAME                0x00000001U
// #define  I2C_FIRST_AND_NEXT_FRAME       0x00000002U
// #define  I2C_NEXT_FRAME                 0x00000004U
// #define  I2C_FIRST_AND_LAST_FRAME       0x00000008U
// #define  I2C_LAST_FRAME_NO_STOP         0x00000010U
// #define  I2C_LAST_FRAME                 0x00000020U

/** @defgroup I2C_Memory_Address_Size I2C Memory Address Size
  * @{
  */
// #define I2C_MEMADD_SIZE_8BIT            0x00000001U
// #define I2C_MEMADD_SIZE_16BIT           0x00000010U


int8_t sBSP_I2C1_Init(uint32_t ClkSpdHz);

int8_t sBSP_I2C2_Init(uint32_t ClkSpdHz);

//通用操作
HAL_StatusTypeDef sBSP_I2C1M_SendByte     (uint16_t DevAddr,uint8_t data);
HAL_StatusTypeDef sBSP_I2C1M_SendBytes    (uint16_t DevAddr,uint8_t* pData,uint16_t length);
uint8_t           sBSP_I2C1M_ReadByte     (uint16_t DevAddr);
HAL_StatusTypeDef sBSP_I2C1M_ReadBytes    (uint16_t DevAddr,uint8_t* pData,uint16_t length);
//顺序操作
HAL_StatusTypeDef sBSP_I2C1M_SeqReadByte  (uint16_t DevAddr,uint8_t* pData,uint32_t XferOptions);
HAL_StatusTypeDef sBSP_I2C1M_SeqReadBytes (uint16_t DevAddr,uint8_t* pData,uint16_t length,uint32_t XferOptions);
HAL_StatusTypeDef sBSP_I2C1M_SeqSendByte  (uint16_t DevAddr,uint8_t data,uint32_t XferOptions);
HAL_StatusTypeDef sBSP_I2C1M_SeqSendBytes (uint16_t DevAddr,uint8_t* pData,uint16_t length,uint32_t XferOptions);
//内存操作
HAL_StatusTypeDef sBSP_I2C1M_MemSendByte  (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size,uint8_t data);
HAL_StatusTypeDef sBSP_I2C1M_MemSendBytes (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size,uint8_t* pData,uint16_t length);
uint8_t           sBSP_I2C1M_MemReadByte  (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size);
HAL_StatusTypeDef sBSP_I2C1M_MemReadBytes (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size,uint8_t* pData,uint16_t length);

//! 这个好像是发送完一帧的完成回调
int8_t sBSP_I2C2M_IsTxCplt();
int8_t sBSP_I2C2M_IsRxCplt();
int8_t sBSP_I2C2M_IsCplt();

int8_t  sBSP_I2C2M_SendByte     (uint16_t DevAddr,uint8_t data);
int8_t  sBSP_I2C2M_SendBytes    (uint16_t DevAddr,uint8_t* pData,uint16_t length);
uint8_t sBSP_I2C2M_ReadByte     (uint16_t DevAddr);
int8_t  sBSP_I2C2M_ReadBytes    (uint16_t DevAddr,uint8_t* pData,uint16_t length);

int8_t  sBSP_I2C2M_SeqReadByte  (uint16_t DevAddr,uint8_t* pData,uint32_t XferOptions);
int8_t  sBSP_I2C2M_SeqReadBytes (uint16_t DevAddr,uint8_t* pData,uint16_t length,uint32_t XferOptions);
int8_t  sBSP_I2C2M_SeqSendByte  (uint16_t DevAddr,uint8_t data,uint32_t XferOptions);
int8_t  sBSP_I2C2M_SeqSendBytes (uint16_t DevAddr,uint8_t* pData,uint16_t length,uint32_t XferOptions);

int8_t  sBSP_I2C2M_MemSendByte  (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size,uint8_t data);
int8_t  sBSP_I2C2M_MemSendBytes (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size,uint8_t* pData,uint16_t length);
uint8_t sBSP_I2C2M_MemReadByte  (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size);
int8_t  sBSP_I2C2M_MemReadBytes (uint16_t dev_addr,uint16_t mem_addr,uint16_t addr_size,uint8_t* pData,uint16_t length);

int8_t sBSP_I2C2M_IsTxCplt();
int8_t sBSP_I2C2M_IsRxCplt();
int8_t sBSP_I2C2M_IsCplt();

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);

    
#ifdef __cplusplus
}
#endif
