#include "sDRV_MB85RCxx.h"
#include "sBSP_I2C.h"

/**
 * sDRV_MB85RCxx.c
 * Sightseer's MB85RCxx铁电驱动
 * 
 * v1.0 2024.04.16 inHNIP9607
 * 目前只支持MB85RC16
 * 
 * v1.1 2024.05.17 inHNIP9607
 * 加入对MB85RC64的支持,优化软件架构,分离了接口,但是MB85RC16不可用(未测试)
 * 
 * 
 * 
 */


//#define MB85RC16
//#define MB85RC16_BYTES    (2048)
#define MB85RC64
#define MB85RC64_BYTES      (8192)

//#define MB85RC256



//铁电的地址
#define MB85RCxx_ADDR       (0x50 << 1)


//****************************************接口******************************************************
static inline void delay(uint16_t ms){
    HAL_Delay(ms);
}
static inline void write_byte(uint16_t addr,uint8_t byte){
    sBSP_I2C1M_MemSendByte(MB85RCxx_ADDR,addr,I2C_MEMADD_SIZE_16BIT,byte);
}
static inline uint8_t read_byte(uint16_t addr){
    return sBSP_I2C1M_MemReadByte(MB85RCxx_ADDR,addr,I2C_MEMADD_SIZE_16BIT);
}
static inline void write_bytes(uint16_t addr,uint8_t* pData,uint16_t length){
    sBSP_I2C1M_MemSendBytes(MB85RCxx_ADDR,addr,I2C_MEMADD_SIZE_16BIT,pData,length);
}
static inline void read_bytes(uint16_t addr,uint8_t* pData,uint16_t length){
    sBSP_I2C1M_MemReadBytes(MB85RCxx_ADDR,addr,I2C_MEMADD_SIZE_16BIT,pData,length);
}

/**
  * @brief  初始化铁电
  *
  * @param  无
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_Init(){
    //数据手册上写可以支持1MHz的I2C速度,实测没问题
    //不需要初始化
    return 0;
}

/**
  * @brief  写一个字节
  *
  * @param  addr 写入的地址
  * @param  byte 数据
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_WriteByte(uint16_t addr,uint8_t byte){
    write_byte(addr,byte);
    return 0;
}

/**
  * @brief  写多个字节
  *
  * @param  addr   起始地址
  * @param  pData  数据指针
  * @param  length 数据长度
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_WriteBytes(uint16_t addr,uint8_t* pData,uint16_t length){
    write_bytes(addr,pData,length);
    return 0;
}

/**
  * @brief  读一个字节
  *
  * @param  addr   地址
  *
  * @return 读到的数据
  */
uint8_t sDRV_MB85RCxx_ReadByte(uint16_t addr){
    return read_byte(addr);
}

/**
  * @brief  读多个字节
  *
  * @param  addr   起始地址
  * @param  pData  数据指针
  * @param  length 数据长度
  *
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_ReadBytes(uint16_t addr,uint8_t* pData,uint16_t length){
    read_bytes(addr,pData,length);
    return 0;
}

/**
  * @brief  格式化铁电
  *
  * @param  all_val : 格式化的值(让所有值都等于)
  * @return 返回0表示正常
  */
int8_t sDRV_MB85RCxx_Format(uint8_t all_val){
    #ifdef MB85RC16
        for(uint32_t i = 0;i < MB85RC16_BYTES;i++){
            sDRV_MB85RCxx_WriteByte(i,all_val);
        }
    #endif

    #ifdef MB85RC64
        for(uint32_t i = 0;i < MB85RC64_BYTES;i++){
            sDRV_MB85RCxx_WriteByte(i,all_val);
        }
    #endif

    return 0;
}

