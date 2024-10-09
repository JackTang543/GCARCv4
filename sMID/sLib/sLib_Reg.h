#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "sLib.h"

//获取
#define SLIB_GET_REG_BIT(__REG__,__REG_MSK__)       ((__REG__) &  (  __REG_MSK__))
//置位
#define SLIB_SET_REG_BIT(__REG__,__REG_MSK__)    do{((__REG__) |= (  __REG_MSK__));}while(0)
//清除
#define SLIB_CLR_REG_BIT(__REG__,__REG_MSK__)    do{((__REG__) &= (~ __REG_MSK__));}while(0)

//设置寄存器的特定位为给定值,注意这个__DATA__要与掩码对齐
#define SLIB_MODIFY_REG_BIT(__REG__, __REG_MSK__, __DATA__) \
    do { \
        (__REG__) = ((__REG__) & ~(__REG_MSK__)) | ((__DATA__) & (__REG_MSK__)); \
    } while(0)



void sLib_ModifyReg(uint8_t* reg_addr, uint8_t mask, uint8_t data);



#ifdef __cplusplus
}
#endif


