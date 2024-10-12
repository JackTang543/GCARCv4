#ifndef __SAPP_FONT_H__
#define __SAPP_FONT_H__
//控制是否启用这个模块
#define __SAPP_FONT_EN__
#ifdef  __SAPP_FONT_EN__

#include "stm32f4xx_hal.h"


typedef struct{
    uint8_t CharByte0;
    uint8_t CharByte1;
    uint8_t CharByte2;
    uint8_t CharByte3;
    uint8_t CharByte4;
}sCGRAM_Char_t;

extern const unsigned char sAPP_Font_8x8Icon_2D4Full[];
extern const unsigned char sAPP_Font_8x8Icon_2D4Half[];
extern const unsigned char sAPP_Font_8x8Icon_Wifi[];
extern const unsigned char sAPP_Font_8x8Icon_degC[];
extern const unsigned char sAPP_Font_8x8Icon_Bat0[];
extern const unsigned char sAPP_Font_8x8Icon_Bat1[];
extern const unsigned char sAPP_Font_8x8Icon_Bat2[];
extern const unsigned char sAPP_Font_8x8Icon_Bat3[];

extern const unsigned char sAPP_Font_Img_WifiInfo[];
extern const unsigned char sAPP_Font_Img_2D4GhzInfo[];
extern const unsigned char sAPP_Font_Img_2D4GhzSettings[];
extern const unsigned char sAPP_Font_Img_WiFiSettings[];
extern const unsigned char sAPP_Font_Img_Settings[];
extern const unsigned char sAPP_Font_Img_StartPage[];
extern const unsigned char sAPP_Font_Img_MainPage[];
extern const unsigned char sAPP_Font_Img_ScrrenBrightness[];
extern const unsigned char sAPP_Font_Img_ScrrenSleepTime[];
extern const unsigned char sAPP_Font_Img_WiFiConnected[];
extern const unsigned char sAPP_Font_Img_WiFiDisconnect[];
extern const unsigned char sAPP_Font_Img_SyncNTPTime[];
extern const unsigned char sAPP_Font_Img_WiFiConnecting[];
extern const unsigned char sAPP_Font_Img_LEDBrightnessAdj[];
extern const unsigned char sAPP_Font_Img_BuzzerVolumeAdj[];


/* 
*  sAPP_Font.h
*  这是一个字库
*  部分由粟禛恺完成  
*
*  v1.0:第一版
*  By Sightseer. 2023.12.18于HNIP-9607Lab    
*  
*/


#endif
#endif
