#include "sGraphic2D.h"

#include <stdarg.h>
#include <stdio.h>

/**
 * sGraphic2D.c
 * Sightseer's 2D Graphic Library
 * Sightseer用于OLED的2D图形库
 * 
 * 
 * 更新记录
 * v1.0 TIME 忘了
 * 初版
 * 
 * v1.1 TIME 2024.06.17inHNIP9607Lab
 * 优化性能,使用DMA
 * 
 * 
 */




//显存,1024Bytes=1KB=256Words=64次DMA 4Inc传输
uint8_t GRAM[128][8];


//字体
extern sCGRAM_Char_t SymbolFont[];
extern sCGRAM_Char_t CharFont[];

uint8_t is_oled_show = 1;


//打点
void sG2D_SetDot(uint8_t x, uint8_t y, uint8_t dot_en){
    if ((x >= 128) || (y >= 64)) {
        return; // 坐标超出范围
    }
    if (dot_en) {
        GRAM[x][y / 8] |= 1 << (y % 8);
    } else {
        GRAM[x][y / 8] &= ~(1 << (y % 8));
    }
}

bool sG2D_GetDot(uint8_t x, uint8_t y){
    if ((x >= 128) || (y >= 64)) {
        return 0; // 坐标超出范围
    }
    return (GRAM[x][y / 8] & (1 << (y % 8))) != 0;
}


void sG2D_RevRectArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
    uint8_t x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            uint8_t currentDot = sG2D_GetDot(x, y);
            sG2D_SetDot(x, y, !currentDot);  // 翻转像素状态
        }
    }
}

//写入一竖列(8个点)
void sG2D_SetByte(uint8_t x, uint8_t y, uint8_t data){
    for(uint8_t i = 0;i < 8;i++){
        if(data & (1 << i)){
            sG2D_SetDot(x,y + i,1);
        }
    }
}

void sG2D_Draw8x8Icon(uint8_t x, uint8_t y,const uint8_t* icon){
    for(uint8_t _x = x;_x < x + 8;_x++){
        sG2D_SetByte(_x, y,icon[_x - x]);
    }
}

void sG2D_DrawScreenByImg(const uint8_t* img){
    for(uint8_t y = 0;y < 8;y++){
        for(uint8_t x = 0;x < 128;x++){
            GRAM[x][y] = img[x + 128 * y];
        }
    }
}

static void WriteUserChar(uint8_t x,uint8_t y,sCGRAM_Char_t user_char){
    sG2D_SetByte(x + 1,y,user_char.CharByte0);
    sG2D_SetByte(x + 2,y,user_char.CharByte1);
    sG2D_SetByte(x + 3,y,user_char.CharByte2);
    sG2D_SetByte(x + 4,y,user_char.CharByte3);
    sG2D_SetByte(x + 5,y,user_char.CharByte4);
}

void sG2D_WriteNumber(uint8_t x,uint8_t y,uint32_t num){
    uint8_t length = (num == 0)? 1 : (uint8_t)(log10(num) + 1);
    for(uint8_t i = length;i > 0;i--){
        WriteUserChar(x + (i * 6) - 6,y,SymbolFont[num / (uint16_t)powf(10 , length - i) % 10]);
    }
}

void sG2D_WriteString(uint8_t x,uint8_t y,const char* str){
    //遍历字符串
    while(*str != '\0'){
        if(x != 128){
            sG2D_SetByte(x + 1,y,CharFont[*str].CharByte0);
            sG2D_SetByte(x + 2,y,CharFont[*str].CharByte1);
            sG2D_SetByte(x + 3,y,CharFont[*str].CharByte2);
            sG2D_SetByte(x + 4,y,CharFont[*str].CharByte3);
            sG2D_SetByte(x + 5,y,CharFont[*str].CharByte4);
            x+=6;
            str++; 
        }else{
            //已经达到所能显示的最大范围了
            return;
        }
    }
}

void sG2D_Printf(uint8_t x,uint8_t y,const char* fmt,...){
    char buf[64] = {0};
    va_list args;
    va_start(args, fmt);
    vsprintf(buf,fmt,args);
    va_end(args);
    
    sG2D_WriteString(x,y,buf);
}


//绘制任意线段
void sG2D_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t dot_en){
    int16_t dx = abs(x1 - x0);
    int16_t dy = -abs(y1 - y0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;

    while (1) {
        sG2D_SetDot(x0, y0, dot_en); // 设置像素点
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void sG2D_DrawVLine(uint8_t x, uint8_t y0, uint8_t y1, uint8_t dot_en){
    for(uint8_t i = y0; i < y1; i++){
        sG2D_SetDot(x,i,dot_en);
    }
}

void sG2D_DrawHLine(uint8_t x0, uint8_t x1,uint8_t y, uint8_t dot_en){
    for(uint8_t i = x0; i < x1; i++){
        sG2D_SetDot(i,y,dot_en);
    }
}

//绘制三角形
void sG2D_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot_en){
    sG2D_DrawLine(x0, y0, x1, y1, dot_en);
    sG2D_DrawLine(x1, y1, x2, y2, dot_en);
    sG2D_DrawLine(x2, y2, x0, y0, dot_en);
}

//绘制矩形
void sG2D_DrawRectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t is_fill){
    if (is_fill) {
        for (uint8_t y = y0; y <= y1; y++) {
            sG2D_DrawHLine(x0, x1, y, 1);
        }
    } else {
        for (uint8_t y = y0 - 1; y < y1; y++) {
            sG2D_DrawHLine(x0 - 0, x1 - 0, y, 0);
        }
        sG2D_DrawHLine(x0, x1, y0, 1);
        sG2D_DrawHLine(x0, x1, y1, 1);
        sG2D_DrawVLine(x0, y0, y1, 1);
        sG2D_DrawVLine(x1, y0, y1 + 1, 1);
    }
}



uint32_t fps_ts_last;
uint32_t fps_ts_curr;
uint32_t fps_prid;
uint32_t fps_val;
uint8_t fps_x = 92;
uint8_t fps_y = 56;
//uint8_t fps_x = 86;
//uint8_t fps_y = 56;

void sG2D_ShowFPS(){
    // sHMI_OLED_WriteString(fps_x,fps_y,"FPS:");
    // sHMI_OLED_WriteNumber(fps_x + 23,fps_y,fps_val);
    sG2D_WriteString(fps_x,fps_y,"FPS:");
    sG2D_WriteNumber(fps_x + 23,fps_y,fps_val);
}

void sG2D_SetAllGRAM(uint8_t px_en){
    for(uint8_t x = 0; x < 128; x++){
        for(uint8_t y = 0; y < 64; y++){
            sG2D_SetDot(x,y,px_en);
        }
    }
}

//使用DMA加速,实测100M主频下只需要10us左右
void sG2D_FastSetAllGRAM(uint8_t px_en){
    //sBSP_DMA2S0_32MemSet(px_en,(uint32_t*)GRAM,128 * 8);
}

void sG2D_SetOLEDShow(uint8_t en){
    is_oled_show = en;
}

void sG2D_UpdateScreen(){
    fps_ts_last = fps_ts_curr;
    fps_ts_curr = HAL_GetTick();
    //fps_ts_curr = millis();
    fps_val = 1000 / (fps_ts_curr - fps_ts_last);


    sDRV_GenOLED_SetShowEN(is_oled_show);
    
    sG2D_DrawRectangle(fps_x - 1,fps_y - 1 ,128,64,0);
    sG2D_ShowFPS();
    sG2D_RevRectArea(fps_x - 0,fps_y - 0 ,128,64);

    sDRV_GenOLED_UpdateScreen();
    //sDRV_GenOLED_FastUpdateScreen();
}


