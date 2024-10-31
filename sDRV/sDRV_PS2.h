#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include "sBSP_SPI.h"
#include "sGCARCv4_Def.h"
#include <stdbool.h>


typedef struct{
    bool select,l3,r3,start,up,right,down,left;
    bool l2,r2,l1,r1,tri,cir,x,rect;
    uint8_t key1;
    uint8_t key2;
    uint8_t leftX,leftY;
    uint8_t rightX,rightY;
}sDRV_PS2_t;

extern sDRV_PS2_t ps2;



int sDRV_PS2_Init();
void sDRV_PS2_Handler();




#ifdef __cplusplus
}
#endif
